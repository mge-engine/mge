// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_block.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/uniform.hpp"

#include <algorithm>
#include <cstring>

namespace mge {
    MGE_USE_TRACE(GRAPHICS);

    uniform_block::uniform_block(
        const program::uniform_block_metadata& buffer_info)
        : m_name(buffer_info.name)
    {
        compute_layout(buffer_info);
        m_data = ::mge::malloc(m_data_size);
        std::memset(m_data, 0, m_data_size);
    }

    uniform_block::~uniform_block()
    {
        if (m_data) {
            ::mge::free(m_data);
            m_data = nullptr;
        }
    }

    uniform_block::uniform_block(uniform_block&& other) noexcept
        : m_name(std::move(other.m_name))
        , m_members(std::move(other.m_members))
        , m_data(other.m_data)
        , m_data_size(other.m_data_size)
        , m_version(other.m_version)
        , m_uniform_cache(std::move(other.m_uniform_cache))
        , m_cache_registry_generation(other.m_cache_registry_generation)
    {
        other.m_data = nullptr;
        other.m_data_size = 0;
    }

    uniform_block& uniform_block::operator=(uniform_block&& other) noexcept
    {
        if (this != &other) {
            if (m_data) {
                ::mge::free(m_data);
            }
            m_name = std::move(other.m_name);
            m_members = std::move(other.m_members);
            m_data = other.m_data;
            m_data_size = other.m_data_size;
            m_version = other.m_version;
            m_uniform_cache = std::move(other.m_uniform_cache);
            m_cache_registry_generation = other.m_cache_registry_generation;
            other.m_data = nullptr;
            other.m_data_size = 0;
        }
        return *this;
    }

    void uniform_block::compute_layout(
        const program::uniform_block_metadata& buffer_info)
    {
        m_members.clear();
        size_t offset = 0;

        for (const auto& u : buffer_info.uniforms) {
            size_t   base_align = std140_base_alignment(u.type);
            uint32_t arr_size = (u.array_size == 0) ? 1 : u.array_size;
            size_t   member_size;

            if (arr_size > 1) {
                size_t stride = std140_array_stride(u.type);
                // Array alignment is max(16, base_align) per std140
                size_t arr_align = (base_align < 16) ? 16 : base_align;
                offset = std140_align(offset, arr_align);
                member_size = stride * arr_size;
            } else {
                offset = std140_align(offset, base_align);
                member_size = std140_type_size(u.type);
            }

            m_members.push_back(
                {u.name, u.type, arr_size, offset, member_size});
            offset += member_size;
        }

        // std140: total block size rounded up to vec4 (16 bytes)
        m_data_size = std140_align(offset, 16);
    }

    void uniform_block::set_data(const std::string& member_name,
                                 const void*        data,
                                 size_t             size)
    {
        auto it = std::find_if(
            m_members.begin(),
            m_members.end(),
            [&](const member_layout& m) { return m.name == member_name; });

        if (it == m_members.end()) {
            MGE_THROW(mge::no_such_element)
                << "Uniform block '" << m_name << "' has no member named '"
                << member_name << "'";
        }

        if (size > it->size) {
            MGE_THROW(mge::out_of_range)
                << "Data size " << size << " exceeds member '" << member_name
                << "' size " << it->size;
        }

        std::memcpy(static_cast<char*>(m_data) + it->offset, data, size);
        ++m_version;
    }

    void uniform_block::ensure_uniform_cache()
    {
        uint64_t current_generation = uniform_base::registry_generation();

        if (!m_uniform_cache.empty() &&
            m_cache_registry_generation == current_generation) {
            return;
        }

        // Reset checked flags when registry changed to re-verify all uniforms
        if (m_cache_registry_generation != current_generation) {
            for (auto& cache : m_uniform_cache) {
                cache.checked = false;
            }
        }

        update_uniform_cache();
        m_cache_registry_generation = current_generation;
    }

    void uniform_block::update_uniform_cache()
    {
        if (m_uniform_cache.empty()) {
            m_uniform_cache.resize(m_members.size());
        }

        for (size_t i = 0; i < m_members.size(); ++i) {
            const auto& member = m_members[i];
            auto&       cache = m_uniform_cache[i];

            // Skip if already checked in this generation
            if (cache.checked) {
                continue;
            }

            uniform_base* global_uniform = uniform_base::find(member.name);
            cache.checked = true;

            if (!global_uniform) {
                cache.uniform = nullptr;
                continue;
            }

            if (global_uniform->type() != member.type) {
                MGE_WARNING_TRACE(
                    GRAPHICS,
                    "Type mismatch for uniform '{}': global={}, block={}",
                    member.name,
                    static_cast<int>(global_uniform->type()),
                    static_cast<int>(member.type));
                cache.uniform = nullptr;
                continue;
            }

            cache.uniform = global_uniform;
            cache.data = global_uniform->data();
            cache.data_size = global_uniform->data_size();
            cache.last_version = 0; // Force initial sync
        }
    }

    void uniform_block::sync_from_globals()
    {
        ensure_uniform_cache();

        bool changed = false;
        for (size_t i = 0; i < m_members.size(); ++i) {
            const auto& member = m_members[i];
            auto&       cache = m_uniform_cache[i];

            if (!cache.uniform) {
                continue;
            }

            uint64_t current_version = cache.uniform->version();
            if (current_version == cache.last_version) {
                continue;
            }

            // Copy value from global uniform to block member
            std::memcpy(static_cast<char*>(m_data) + member.offset,
                        cache.data,
                        cache.data_size);
            cache.last_version = current_version;
            changed = true;
        }

        if (changed) {
            ++m_version;
        }
    }

} // namespace mge
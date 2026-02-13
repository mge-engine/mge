// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform_block.hpp"
#include "mge/core/memory.hpp"
#include "mge/core/stdexceptions.hpp"

#include <algorithm>
#include <cstring>

namespace mge {

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

} // namespace mge
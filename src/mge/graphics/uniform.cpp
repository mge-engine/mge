// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/string_pool.hpp"

#include <unordered_map>

namespace mge {
    class uniform_registry
    {
    public:
        uniform_registry()
            : m_mutex("uniform_registry")
        {}

        std::string_view register_uniform(const std::string_view name,
                                          uniform_base*          uniform)
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            auto                        pooled_name = m_string_pool.get(name);
            auto [_, inserted] = m_uniforms.emplace(pooled_name, uniform);
            if (!inserted) {
                MGE_THROW(mge::duplicate_element)
                    << "Uniform registration failed: "
                    << "An uniform with the name '" << pooled_name
                    << "' is already registered.";
            }
            ++m_generation;
            return pooled_name;
        }

        void unregister_uniform(const std::string_view name)
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            m_uniforms.erase(name);
            ++m_generation;
        }

        uniform_base* find(const std::string_view& name) const
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            auto                        it = m_uniforms.find(name);
            if (it == m_uniforms.end()) {
                return nullptr;
            }
            return it->second;
        }

        uint64_t generation() const
        {
            std::lock_guard<mge::mutex> lock(m_mutex);
            return m_generation;
        }

    private:
        mutable mge::mutex m_mutex;
        string_pool        m_string_pool;
        std::unordered_map<std::string_view,
                           uniform_base*,
                           std::hash<std::string_view>>
                 m_uniforms;
        uint64_t m_generation{0};
    };

    mge::singleton<uniform_registry> s_uniform_registry;

    uniform_base::uniform_base(const std::string_view& name)
        : m_name(s_uniform_registry->register_uniform(name, this))
    {}

    uniform_base::~uniform_base()
    {
        s_uniform_registry->unregister_uniform(m_name);
    }

    uniform_base* uniform_base::find(const std::string_view& name)
    {
        return s_uniform_registry->find(name);
    }

    uint64_t uniform_base::registry_generation()
    {
        return s_uniform_registry->generation();
    }
} // namespace mge
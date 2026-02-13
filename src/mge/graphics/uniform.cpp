// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/uniform.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/string_pool.hpp"

#include <unordered_map>

namespace mge {
    class uniform_registry
    {
    public:
        uniform_registry() = default;

        std::string_view register_uniform(const std::string_view name,
                                          uniform_base*          uniform)
        {
            auto pooled_name = m_string_pool.get(name);
            auto [_, inserted] = m_uniforms.emplace(pooled_name, uniform);
            if (!inserted) {
                MGE_THROW(mge::duplicate_element)
                    << "Uniform registration failed: "
                    << "An uniform with the name '" << pooled_name
                    << "' is already registered.";
            }
            return pooled_name;
        }

        void unregister_uniform(const std::string_view name)
        {
            m_uniforms.erase(name);
        }

    private:
        string_pool m_string_pool;
        std::unordered_map<std::string_view,
                           uniform_base*,
                           std::hash<std::string_view>>
            m_uniforms;
    };

    mge::singleton<uniform_registry> s_uniform_registry;

    uniform_base::uniform_base(const std::string_view& name)
        : m_name(s_uniform_registry->register_uniform(name, this))
    {}

    uniform_base::~uniform_base()
    {
        s_uniform_registry->unregister_uniform(m_name);
    }
} // namespace mge
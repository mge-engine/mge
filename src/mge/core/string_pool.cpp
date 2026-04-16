// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/string_pool.hpp"
#include <cstring>
#include <stdexcept>

namespace mge {
    string_pool::string_pool(std::pmr::memory_resource* resource)
        : m_values(resource)
    {}

    std::string_view string_pool::get(std::string_view str)
    {
        auto*            resource = m_values.get_allocator().resource();
        std::pmr::string key(str, resource);
        auto             it = m_values.find(key);
        if (it != m_values.end()) {
            return std::string_view(it->second);
        } else {
            std::pmr::string value(str, resource);
            auto [pos, inserted] =
                m_values.emplace(std::move(key), std::move(value));
            return std::string_view(pos->second);
        }
    }

    std::string_view string_pool::get(const std::string& str)
    {
        return get(std::string_view(str));
    }

    std::string_view string_pool::get(const char* str)
    {
        if (!str) {
            throw std::runtime_error("Unexpected null pointer");
        }
        return get(std::string_view(str, strlen(str)));
    }

    string_pool::size_type string_pool::size() const
    {
        return m_values.size();
    }

} // namespace mge
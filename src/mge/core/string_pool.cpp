// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/string_pool.hpp"
#include <cstring>
#include <stdexcept>

namespace mge {
    string_pool::string_pool() {}

    std::string_view string_pool::get(std::string_view str)
    {
        auto it = m_values.find(str);
        if (it != m_values.end()) {
            return it->first;
        } else {
            auto value = std::make_shared<std::string>(str.begin(), str.end());
            std::string_view key(value->begin(), value->end());
            m_values.emplace(key, std::move(value));
            return key;
        }
    }

    std::string_view string_pool::get(const std::string& str)
    {
        std::string_view str_sv(str.begin(), str.end());
        return get(str_sv);
    }

    std::string_view string_pool::get(const char* str)
    {
        if (!str) {
            throw std::runtime_error("Unexpected null pointer");
        }

        size_t l = strlen(str);

        std::string_view str_sv(str, str + l);
        return get(str_sv);
    }

    string_pool::size_type string_pool::size() const
    {
        return m_values.size();
    }

} // namespace mge
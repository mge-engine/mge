// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/string_pool.hpp"
#include "mge/core/exception.hpp"

namespace mge {

    const char *string_pool::intern(const char *str)
    {
        auto it = m_pool_set.find(str);
        if (it != m_pool_set.end()) {
            return *it;
        } else {
            return insert_element(std::string(str));
        }
    }

    const char *string_pool::intern(const std::string &str)
    {
        auto it = m_pool_set.find(str.c_str());
        if (it != m_pool_set.end()) {
            return *it;
        } else {
            return insert_element(std::string(str));
        }
    }

    const char *string_pool::intern(std::string &&str)
    {
        auto it = m_pool_set.find(str.c_str());
        if (it != m_pool_set.end()) {
            return *it;
        } else {
            return insert_element(std::move(str));
        }
    }

    const char *string_pool::insert_element(std::string &&str)
    {
        m_pool.emplace_back(std::make_unique<std::string>(std::move(str)));
        try {
            auto result = m_pool.back().get()->c_str();
            m_pool_set.insert(result);
            return result;
        } catch (...) {
            m_pool.pop_back();
            mge::rethrow();
        }
    }
} // namespace mge

// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/string_pool.hpp"

namespace mge {

    string_pool::string_pool()
    {}

    string_pool::~string_pool()
    {
        for(const auto e : m_pool) {
            delete e;
        }
    }

    const char *
    string_pool::intern(const char *str)
    {
        auto it = m_pool_set.find(str);
        if (it != m_pool_set.end()) {
            return *it;
        } else {
            return insert_element(str);
        }
    }

    const char *
    string_pool::intern(const std::string& str)
    {
        auto it = m_pool_set.find(str.c_str());
        if (it != m_pool_set.end()) {
            return *it;
        } else {
            return insert_element(str.c_str());
        }
    }


    const char *
    string_pool::insert_element(const char *str)
    {
        std::string *ins = nullptr;
        try {
            ins = new std::string(str);
            m_pool.push_back(ins);
            try {
                m_pool_set.insert(ins->c_str());
            } catch(const std::exception&) {
                m_pool.pop_back();
                throw;
            }
            return ins->c_str();
        } catch(const std::exception&) {
            delete ins;
            throw;
        }
    }
}

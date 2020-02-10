// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/types.hpp"
#include "mge/core/memory.hpp"
#include <vector>
#include <set>
#include <string>
#include <cstring>

/**
 * @file mge/core/string_pool.hpp
 * @brief String pool class.
 */

namespace mge {

    /**
     * @brief A string pool.
     *
     * A string pool keeps strings so that they can be processed by
     * just using @c char pointers.
     */
    class MGE_CORE_EXPORT string_pool
            : mge::noncopyable
    {
    public:
        /**
         * Constructor.
         */
        string_pool() = default;
        
        /**
         * Destructor.
         */
        ~string_pool() = default;

        /**
         * Returns a canonical representation of the string @c str
         * within this string pool.
         * @param str string to intern
         * @return string with same contents as str, contained in the
         *   string pool
         */
        const char *intern(const char *str);

        /**
         * Returns a canonical representation of the string @c str
         * within this string pool.
         * @param str string to intern
         * @return string with same contents as str, contained in the
         *   string pool
         */
        const char *intern(const std::string& str);

        /**
         * Returns a canonical representation of the string @c str
         * within this string pool.
         * @param str string to intern
         * @return string with same contents as str, contained in the
         *   string pool
         */
        const char *intern(std::string&& str);
    private:
        const char *insert_element(std::string&& str);

        struct string_less
        {
            bool operator ()(const char *a, const char *b) const
            {
                return strcmp(a, b) < 0;
            }
        };

        std::vector<std::unique_ptr<std::string>> m_pool;
        std::set<const char *, string_less> m_pool_set;
    };
}

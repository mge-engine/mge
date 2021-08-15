// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <map>
#include <memory>
#include <string>
#include <string_view>
namespace mge {

    /**
     * @brief String pool.
     *
     * Manages stable string views.
     */
    class MGECORE_EXPORT string_pool
    {
    private:
        typedef std::map<std::string_view, std::shared_ptr<std::string>>
            string_map;

    public:
        /// Size of pool.
        using size_type = string_map::size_type;

        /**
         * @brief Create empty pool.
         */
        string_pool();

        /**
         * @brief Get pooled string.
         *
         * @param str string not in pool
         * @return pooled string
         */
        std::string_view get(std::string_view str);
        /**
         * @brief Get pooled string.
         *
         * @param str string not in pool
         * @return pooled string
         */
        std::string_view get(const std::string& str);
        /**
         * @brief Get pooled string.
         *
         * @param str string not in pool
         * @return pooled string
         */
        std::string_view get(const char* str);

        /**
         * @brief Get pool size.
         *
         * @return pool size
         */
        size_type size() const;

    private:
        string_map m_values;
    };

} // namespace mge
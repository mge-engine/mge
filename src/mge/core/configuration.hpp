// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <string_view>

namespace mge {

    class basic_parameter;
    /**
     * @brief Configuration management.
     *
     */
    class MGECORE_EXPORT configuration
    {
    public:
        static void register_parameter(basic_parameter& p);
        static void unregister_parameter(basic_parameter& p);

        /**
         * @brief Find a parameter,
         *
         * @param section   parameter section
         * @param name      parameter name
         * @return parameter as registered, a  @c std::runtime_error is
         * thrown if parameter is not found
         */
        static basic_parameter& find_parameter(std::string_view section, std::string_view name);
        /**
         * @brief Load configuration.
         * @param allow_missing succeeds even if no configuration is found
         */
        static void load(bool allow_missing = true);
        /**
         * @brief Store configuration.
         */
        static void store();

        /**
         * @brief Return whether configuration already has been loaded.
         * @return @c true if already loaded
         */
        static bool loaded();
    };

} // namespace mge
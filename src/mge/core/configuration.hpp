// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/json.hpp"
#include "mge/core/path.hpp"
#include "mge/core/stdexceptions.hpp"

#include <string_view>
#include <vector>

namespace mge {

    class basic_parameter;
    /**
     * @brief Configuration management.
     *
     */
    class MGECORE_EXPORT configuration
    {
    public:
        /**
         * @brief Register parameter for configuration management.
         * @param p parameter to register
         */
        static void register_parameter(basic_parameter& p);
        /**
         * @brief Unregister parameter from configuration management.
         * @param p parameter to unregister
         */
        static void unregister_parameter(basic_parameter& p);

        /**
         * @brief Find a parameter,
         *
         * @param section   parameter section
         * @param name      parameter name
         * @return parameter as registered, a  @c std::runtime_error is
         * thrown if parameter is not found
         */
        static basic_parameter& find_parameter(std::string_view section,
                                               std::string_view name);

        /**
         *  Access to parameter by section and name.
         * @param section   parameter section
         * @param name      parameter name
         * @return parameter as registered
         */
        static auto& get(std::string_view section, std::string_view name)
        {
            return find_parameter(section, name);
        }

        /**
         * @brief Find a parameter,
         *
         * @param section   parameter section
         * @param name      parameter name
         * @return parameter as registered, a  @c std::runtime_error is
         * thrown if parameter is not found
         */
        static basic_parameter& find_parameter(const mge::path& path);

        /**
         * @brief Load configuration. Configuration file name is derived from
         * executable name.
         */
        static void load();

        /**
         * @brief Load configuration from specific file.
         *
         * @param config_name configuration base name to use instead of program
         * name
         */
        static void load(const std::string& config_name);

        /**
         * @brief Evaluates the command line for configuration changes.
         *
         * @param cmdline command line
         */
        static void
        evaluate_command_line(const std::vector<const char*>& cmdline);

        /**
         * @brief Store configuration.
         */
        static void store();

        /**
         * @brief Return whether configuration already has been loaded.
         * @return @c true if already loaded
         */
        static bool loaded();

        /**
         * @brief Root configuration.
         */
        static const mge::json::json& root();
    };

    /** @brief Throw if a configuration does not fulfill constraints. */
    MGECORE_DECLARE_EXCEPTION_CLASS(bad_configuration);

} // namespace mge
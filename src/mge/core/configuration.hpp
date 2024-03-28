// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/path.hpp"

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
        static basic_parameter& find_parameter(std::string_view section,
                                               std::string_view name);

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
         * @brief Load configuration.
         */
        static void load();

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
        class element;

        using element_ref = std::shared_ptr<element>;

        class element
        {
        public:
            /**
             * @brief Contained data type.
             */
            enum class data_type
            {
                TYPE_VALUE,  //!< single value
                TYPE_ARRAY,  //!< array of values
                TYPE_OBJECT, //!< object, i.e. key-value pairs
                TYPE_NULL    //!< null value
            };

            /**
             * @brief Destroy the element object
             */
            virtual ~element() = default;
            /**
             * @brief Get the child object.
             *
             * @param name key of child
             * @return refernce to child, null reference if not found
             */
            virtual element_ref child(const std::string& name) const = 0;
            /**
             * @brief Get element data type.
             * @return data type
             */
            virtual data_type type() const = 0;

            /**
             * @brief Get element value.
             *
             * @return element value
             */
            virtual std::string_view value() const = 0;
        };

        /**
         * @brief Root configuration element.
         *
         * @return element&
         */
        static const element_ref& root();
    };

} // namespace mge
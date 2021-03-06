// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/make_string_view.hpp"
#include <boost/lexical_cast.hpp>

#include <any>
#include <sstream>
#include <string_view>

namespace mge {

    /**
     * @brief Configuration parameter.
     * Implements the interface to describe, read and write a parameter.
     */
    class MGECORE_EXPORT basic_parameter
    {
    public:
        /**
         * @brief Construct a parameter.
         *
         * @param section       parameter section
         * @param name          parameter name
         * @param description   parameter description
         */
        basic_parameter(std::string_view section, std::string_view name,
                        std::string_view description);
        virtual ~basic_parameter();

        /**
         * @brief Parameter section.
         *
         * @return parameter section name
         */
        std::string_view section() const noexcept;
        /**
         * @brief Parameter name.
         *
         * @return parameter name
         */
        std::string_view name() const noexcept;
        /**
         * @brief Parameter description.
         *
         * @return parameter description text
         */
        std::string_view description() const noexcept;

        /**
         * @brief Check whether parameter is set.
         *
         * @return @c true if the parameter is set in
         * configuration
         */
        virtual bool has_value() const = 0;
        /**
         * @brief Unset or clears a parameter.
         */
        virtual void reset() = 0;
        /**
         * @brief Sets parameter from string
         *
         * @param value parameter string value
         */
        virtual void from_string(const std::string &value) = 0;
        /**
         * @brief Retrieves parameter as string.
         *
         * @return parameter value
         */
        virtual std::string to_string() const = 0;

    private:
        std::string_view m_section;
        std::string_view m_name;
        std::string_view m_description;
    };

    /**
     * @brief Typed parameter.
     *
     * @tparam T parameter type
     */
    template <typename T> class parameter : public basic_parameter
    {
    public:
        /**
         * @brief Construct a new parameter.
         *
         * @tparam SECTION_N length of section literal
         * @tparam NAME_N length of name literal
         * @tparam DESCRIPTION_N  length of description literal
         * @param section parameter section
         * @param name parameter name
         * @param description parameter description
         */
        template <size_t SECTION_N, size_t NAME_N, size_t DESCRIPTION_N>
        parameter(const char (&section)[SECTION_N], const char (&name)[NAME_N],
                  const char (&description)[DESCRIPTION_N])
            : basic_parameter(make_string_view(section), make_string_view(name),
                              make_string_view(description))
        {}

        /**
         * @brief Construct a new parameter object
         *
         * @param section parameter section
         * @param name parameter name
         * @param description parameter description
         */
        parameter(std::string_view section, std::string_view name,
                  std::string_view description)
            : basic_parameter(section, name, description)
        {}

        virtual ~parameter() = default;

        bool has_value() const override { return m_value.has_value(); }

        /**
         * @brief Retrieve typed value.
         *
         * @return stored value
         */
        typename T get() const { return std::any_cast<T>(m_value); }

        void from_string(const std::string &value) override
        {
            T val   = boost::lexical_cast<T>(value);
            m_value = val;
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << get();
            return ss.str();
        }

        void reset() override { m_value.reset(); }

    private:
        std::any m_value;
    };

/**
 * @def MGE_DEFINE_PARAMETER
 * @brief Define a parameter.
 * @param TYPE parameter type
 * @param SECTION parameter section
 * @param NAME parameter name
 * @param DESCRIPTION parameter description
 */
#define MGE_DEFINE_PARAMETER(TYPE, SECTION, NAME, DESCRIPTION)                 \
    ::mge::parameter<TYPE> p_##SECTION##_##NAME(#SECTION, #NAME, DESCRIPTION);

/**
 * @def MGE_PARAMETER
 * @brief Access a parameter.
 * @param SECTION parameter section
 * @param NAME parameter name
 */
#define MGE_PARAMETER(SECTION, NAME) p_##SECTION##_##NAME
} // namespace mge
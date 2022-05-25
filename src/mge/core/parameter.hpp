// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_algorithm_string.hpp"
#include "boost/boost_lexical_cast.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/make_string_view.hpp"

#include <any>
#include <functional>
#include <mutex>
#include <sstream>
#include <string_view>
#include <vector>

namespace mge {

    /**
     * @brief Configuration parameter.
     * Implements the interface to describe, read and write a parameter.
     */
    class MGECORE_EXPORT basic_parameter
    {
    public:
        using change_callback = std::function<void()>;

        /**
         * @brief Construct a parameter.
         *
         * @param section       parameter section
         * @param name          parameter name
         * @param description   parameter description
         */
        basic_parameter(std::string_view section,
                        std::string_view name,
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
        virtual void reset();
        /**
         * @brief Sets parameter from string
         *
         * @param value parameter string value
         */
        virtual void from_string(std::string_view value) = 0;
        /**
         * @brief Retrieves parameter as string.
         *
         * @return parameter value
         */
        virtual std::string to_string() const = 0;

        /**
         * @brief Call to notify a change.
         * If a change listener is registered, call that.
         */
        void notify_change();

        /**
         * @brief Return current change handler.
         *
         * @return change handler
         */
        change_callback change_handler() const;

        /**
         * @brief Set the change handler.
         *
         * @param handler new change handler
         */
        void set_change_handler(const change_callback& handler);

    private:
        std::string_view m_section;
        std::string_view m_name;
        std::string_view m_description;

        mutable std::mutex m_change_lock;
        change_callback    m_change_callback;
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
        parameter(const char (&section)[SECTION_N],
                  const char (&name)[NAME_N],
                  const char (&description)[DESCRIPTION_N])
            : basic_parameter(make_string_view(section),
                              make_string_view(name),
                              make_string_view(description))
        {}

        /**
         * @brief Construct a new parameter.
         *
         * @tparam SECTION_N length of section literal
         * @tparam NAME_N length of name literal
         * @tparam DESCRIPTION_N  length of description literal
         * @param section parameter section
         * @param name parameter name
         * @param description parameter description
         * @param default_value
         */
        template <size_t SECTION_N, size_t NAME_N, size_t DESCRIPTION_N>
        parameter(const char (&section)[SECTION_N],
                  const char (&name)[NAME_N],
                  const char (&description)[DESCRIPTION_N],
                  const T& default_value)
            : basic_parameter(make_string_view(section),
                              make_string_view(name),
                              make_string_view(description))
            , m_default_value(default_value)
        {}

        /**
         * @brief Construct a new parameter object
         *
         * @param section parameter section
         * @param name parameter name
         * @param description parameter description
         */
        parameter(std::string_view section,
                  std::string_view name,
                  std::string_view description)
            : basic_parameter(section, name, description)
        {}

        virtual ~parameter() = default;

        bool has_value() const override
        {
            return m_value.has_value() || m_default_value.has_value();
        }

        /**
         * @brief Retrieve typed value.
         *
         * @return stored value
         */
        typename T get() const
        {
            if (m_value.has_value()) {
                return std::any_cast<T>(m_value);
            } else {
                return std::any_cast<T>(m_default_value);
            }
        }

        /**
         * @brief Retrieve typed value, checking default.
         *
         * @param default_value default value
         * @return config value or default value if not set
         */
        typename T get(const T& default_value) const
        {
            if (has_value()) {
                return get();
            } else {
                return default_value;
            }
        }

        void from_string(std::string_view value) override
        {
            T val = boost::lexical_cast<T>(value);
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
        std::any       m_value;
        const std::any m_default_value;
    };

    template <typename T>
    class parameter<std::vector<T>> : public basic_parameter
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
        parameter(const char (&section)[SECTION_N],
                  const char (&name)[NAME_N],
                  const char (&description)[DESCRIPTION_N])
            : basic_parameter(make_string_view(section),
                              make_string_view(name),
                              make_string_view(description))
        {}

        /**
         * @brief Construct a new parameter object
         *
         * @param section parameter section
         * @param name parameter name
         * @param description parameter description
         */
        parameter(std::string_view section,
                  std::string_view name,
                  std::string_view description)
            : basic_parameter(section, name, description)
        {}

        virtual ~parameter() = default;

        bool has_value() const override { return !m_values.empty(); }

        /**
         * @brief Retrieve typed value.
         *
         * @return stored value
         */
        typename const std::vector<T>& get() const { return m_values; }

        /**
         * @brief Retrieve typed value, checking default.
         *
         * @param default_value default value
         * @return config value or default value if not set
         */
        typename const std::vector<T>&
        get(const std::vector<T>& default_value) const
        {
            if (has_value()) {
                return get();
            } else {
                return default_value;
            }
        }

        void from_string(std::string_view value) override
        {
            std::vector<std::string> string_values;
            boost::split(string_values,
                         value,
                         boost::is_any_of(","),
                         boost::token_compress_on);
            for (auto& string_val : string_values) {
                boost::trim(string_val);
            }
            m_values.clear();
            for (const auto& string_val : string_values) {
                m_values.emplace_back(boost::lexical_cast<T>(string_val));
            }
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            const auto&       values = get();
            if (!values.empty()) {
                std::copy(std::begin(values),
                          std::prev(std::end(values)),
                          std::ostream_iterator<T>(ss, ", "));
                ss << values.back();
            }
            return ss.str();
        }

        void reset() override { m_values.clear(); }

    private:
        std::vector<T> m_values;
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
 * @def MGE_DEFINE_PARAMETER_WITH_DEFAULT
 * @brief Define a parameter.
 * @param TYPE parameter type
 * @param SECTION parameter section
 * @param NAME parameter name
 * @param DESCRIPTION parameter description
 * @param DEFAULT
 */
#define MGE_DEFINE_PARAMETER_WITH_DEFAULT(TYPE,                                \
                                          SECTION,                             \
                                          NAME,                                \
                                          DESCRIPTION,                         \
                                          DEFAULT)                             \
    ::mge::parameter<TYPE> p_##SECTION##_##NAME(#SECTION,                      \
                                                #NAME,                         \
                                                DESCRIPTION,                   \
                                                DEFAULT);

/**
 * @def MGE_PARAMETER
 * @brief Access a parameter.
 * @param SECTION parameter section
 * @param NAME parameter name
 */
#define MGE_PARAMETER(SECTION, NAME) p_##SECTION##_##NAME
} // namespace mge
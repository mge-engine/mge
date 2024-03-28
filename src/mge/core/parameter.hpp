// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/is_associative_container.hpp"
#include "mge/core/is_container.hpp"
#include "mge/core/lexical_cast.hpp"
#include "mge/core/make_string_view.hpp"
#include "mge/core/path.hpp"

#include <any>
#include <functional>
#include <mutex>
#include <sstream>
#include <string_view>
#include <variant>
#include <vector>

namespace mge {

    /**
     * @brief Configuration parameter.
     * Implements the interface to describe, read and write a parameter.
     */
    class MGECORE_EXPORT basic_parameter
    {
    public:
        using set_function =
            std::function<void(const mge::configuration::element_ref&)>;
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

        /**
         * @brief Construct a parameter.
         *
         * @param path          path to parameter
         * @param description   parameter description
         */
        basic_parameter(const mge::path path, std::string_view description);

        virtual ~basic_parameter();

        /**
         * @brief return the parameter path.
         *
         * @return parameter path
         */
        const mge::path& path() const;

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
#if 0
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
#endif
        /**
         * @brief Retrieves parameter as string.
         *
         * @return parameter value
         */
        virtual std::string to_string() const = 0;
#if 0
        /**
         * @brief Add a value to a value list parameter.
         *
         * @param value value to add
         */
        virtual void add_value(std::string_view value)
        {
            MGE_THROW(not_implemented) << "Parameter is not a value list";
        }

        /**
         * @brief Method to read out each value in case of a value
         * list parameter.
         *
         * @param f function called
         */
        virtual void
        for_each_value(const std::function<void(const std::string&)>& f) const
        {
            f(to_string());
        }

        virtual void add_value(std::string_view key, std::string_view value)
        {
            MGE_THROW(not_implemented) << "Parameter is not a map";
        }
#endif

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

        void apply(const mge::configuration::element_ref& element);

    protected:
        set_function m_set_function;

    private:
        mge::path        m_path;
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
        {
            init_set_function();
        }

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
        {
            init_set_function();
        }

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
        {
            init_set_function();
        }

        /**
         * @brief Construct a new parameter object
         *
         * @param path parameter path
         * @param description parameter description
         */
        parameter(const mge::path& path, std::string_view description)
            : basic_parameter(path, description)
        {
            init_set_function();
        }

        /**
         * @brief Construct a parameter
         *
         * @param path          path to parameter
         * @param description   parameter description
         * @param default_value default value
         */
        parameter(const mge::path& path,
                  std::string_view description,
                  const T&         default_value)
            : basic_parameter(path, description)
            , m_default_value(default_value)
        {
            init_set_function();
        }

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

        std::string to_string() const override
        {
            std::stringstream ss;
            ss << get();
            return ss.str();
        }

    private:
        void init_set_function()
        {
            m_set_function = [this](const mge::configuration::element_ref& e) {
                if (e) {
                    m_value = mge::lexical_cast<T>(e->value());
                } else {
                    m_value.reset();
                }
            };
        }

        std::any       m_value;
        const std::any m_default_value;
    };

    template <typename T>
        requires mge::is_sequence_container<T>
    class parameter<T> : public basic_parameter
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
         * @param path parameter path
         * @param description parameter description
         */
        parameter(const mge::path& path, std::string_view description)
            : basic_parameter(path, description)
        {}

        virtual ~parameter() = default;

        bool has_value() const override { return !m_values.empty(); }

        std::string to_string() const override
        {
            MGE_THROW(not_implemented) << "Parameter is not a value";
        }

        const T& values() const { return m_values; }

    private:
        T m_values;
    };

#if 0
    template <typename T>
        requires mge::is_associative_container<T> && mge::is_string<T::key_type>
    class parameter<T> : public basic_parameter
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
         * @param path parameter path
         * @param description parameter description
         */
        parameter(const mge::path& path, std::string_view description)
            : basic_parameter(path, description)
        {}

        virtual ~parameter() = default;

        basic_parameter::type value_type() const override
        {
            return basic_parameter::type::MAP
        }

        bool has_value() const override { return !m_values.empty(); }

        void from_string(std::string_view value) override
        {
            MGE_THROW(not_implemented) << "Parameter is not a value";
        }

        std::string to_string() const override
        {
            MGE_THROW(not_implemented) << "Parameter is not a value";
        }

        void add_value(std::string_view value) override
        {
            MGE_THROW(not_implemented) << "Parameter is not a value list";
        }

        void for_each_value(
            const std::function<void(const std::string&)>& f) const override
        {
            MGE_THROW(not_implemented) << "Parameter is not a value list";
        }

        void reset() override { m_values.clear(); }

        const T& values() const { return m_values; }

    private:
        T m_values;
    };
#endif

#if 0

    template <typename T>
        requires mge::is_map<T> && mge::is_string<T::key_type>
    class parameter<T> : public basic_parameter
    {};
#endif

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
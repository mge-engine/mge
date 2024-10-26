// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "boost/boost_algorithm_string.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/json.hpp"
#include "mge/core/lexical_cast.hpp"
#include "mge/core/make_string_view.hpp"
#include "mge/core/path.hpp"
#include "mge/core/reset.hpp"
#include "mge/core/stdexceptions.hpp"

#include "boost/preprocessor.hpp"

#include <functional>
#include <mutex>
#include <optional>
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
        using read_function = std::function<void(const mge::json::json&)>;
        using write_function = std::function<void(mge::json::json&)>;

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

        /**
         * @brief Unset or clears a parameter.
         */
        virtual void reset();

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

        /**
         * @brief Set parameter value from json document.
         *
         * @param document json document
         */
        void read_value(const mge::json::json& document);

        /**
         * @brief Write parameter value to json document.
         *
         * @param document json document
         */
        void write_value(mge::json::json& document) const;

    protected:
        read_function  m_read_function;
        write_function m_write_function;

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
            , m_has_value(false)
        {
            m_read_function = [&](const mge::json::json& j) {
                if (j.is_null() || j.is_discarded()) {
                    reset();
                } else {
                    from_json(j, m_value);
                    m_has_value = true;
                }
            };

            m_write_function = [&](mge::json::json& j) {
                if (has_value()) {
                    to_json(j, m_value);
                }
            };
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
            , m_has_value(false)
        {
            m_read_function = [&](const mge::json::json& j) {
                if (j.is_null() || j.is_discarded()) {
                    reset();
                } else {
                    from_json(j, m_value);
                    m_has_value = true;
                }
            };
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
            , m_has_value(false)
        {
            m_read_function = [&](const mge::json::json& j) {
                if (j.is_null() || j.is_discarded()) {
                    reset();
                } else {
                    from_json(j, m_value);
                    m_has_value = true;
                }
            };
        }

        /**
         * @brief Construct a new parameter object
         *
         * @param path parameter path
         * @param description parameter description
         */
        parameter(const mge::path& path, std::string_view description)
            : basic_parameter(path, description)
            , m_has_value(false)
        {
            m_read_function = [&](const mge::json::json& j) {
                if (j.is_null() || j.is_discarded()) {
                    reset();
                } else {
                    from_json(j, m_value);
                    m_has_value = true;
                }
            };
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
            , m_has_value(false)
        {
            m_read_function = [&](const mge::json::json& j) {
                if (j.is_null() || j.is_discarded()) {
                    reset();
                } else {
                    from_json(j, m_value);
                    m_has_value = true;
                }
            };
        }

        virtual ~parameter() = default;

        bool has_value() const override { return m_has_value; }

        void reset() override
        {
            mge::reset(m_value);
            m_has_value = false;
        }

        T& value()
        {
            m_has_value = true;
            return m_value;
        }

        /**
         * @brief Retrieve typed value.
         *
         * @return stored value
         */
        const T& get() const
        {
            if (has_value()) {
                return m_value;
            } else if (m_default_value.has_value()) {
                return m_default_value.value();
            } else {
                MGE_THROW(illegal_state)
                    << "Parameter " << path() << " has no value";
            }
        }

        /**
         * @brief Retrieve typed value, checking default.
         *
         * @param default_value default value
         * @return config value or default value if not set
         */
        const T& get(const T& default_value) const
        {
            if (has_value()) {
                return get();
            } else {
                return default_value;
            }
        }

    private:
        T                m_value;
        std::optional<T> m_default_value;
        bool             m_has_value;
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
    ::mge::parameter<typename BOOST_PP_REMOVE_PARENS(TYPE)>                    \
        p_##SECTION##_##NAME(#SECTION, #NAME, DESCRIPTION);

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
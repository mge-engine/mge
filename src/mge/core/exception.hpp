// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/format.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/type_name.hpp"
#include <any>
#include <exception>
#include <optional>
#include <source_location>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

namespace mge {

    /**
     * @brief An exception.
     */
    class MGECORE_EXPORT exception : virtual public std::exception
    {
    public:
        /**
         * Helper class for detailed exception information.
         */
        struct exception_details
        {
        public:
            explicit exception_details(const mge::exception* ex) noexcept
                : m_ex(ex)
            {}

            exception_details(const exception_details&) = default;
            exception_details(exception_details&&) = default;
            exception_details& operator=(const exception_details&) = default;
            exception_details& operator=(exception_details&&) = default;

            inline const mge::exception* ex() const noexcept
            {
                return m_ex;
            }

        private:
            const mge::exception* m_ex;
        };

    private:
        struct tag_base
        {};

    public:
        /**
         * @brief Exception value tag type.
         *
         * @tparam Tag type used to access the value
         * @tparam Value stored value type
         *
         * The tag type is used to attach values to the
         * exception.
         *
         * To use it, create a type that will hold the information as a
         * member @c value, and derive that type from the tag type as follows.
         * @code
         * // A 'foo' value will hold a string attached to an exception.
         * struct foo : public tag<foo, std::string>
         * {
         *     std::string value;
         * };
         * @endcode
         */
        template <typename Tag, typename Value> struct tag : public tag_base
        {
            using tag_type = Tag;     //!< Tag type.
            using value_type = Value; //!< Value type of value stored under tag.
        };

        /**
         * @brief Source file name attached to exception.
         */
        struct source_file : public tag<source_file, std::string_view>
        {
            /**
             * @brief Capture source file name.
             * @param value_ source file name
             */
            source_file(const std::string_view& value_) noexcept
                : m_value(value_)
            {}

            std::string_view value() const noexcept
            {
                return m_value;
            }

            std::string_view m_value;
        };

        /**
         * @brief Function name attached to exception.
         */
        struct function : public tag<function, std::string_view>
        {
            /**
             * @brief Capture current function name.
             * @param value_ current function name
             */
            function(const std::string_view& value_) noexcept
                : m_value(value_)
            {}

            std::string_view value() const noexcept
            {
                return m_value;
            }

            std::string_view m_value;
        };

        /**
         * @brief Source file line number attached to exception.
         */
        struct source_line : public tag<source_line, uint32_t>
        {
            /**
             * @brief Capture source line number.
             * @param value_ source line number
             */
            source_line(uint32_t value_) noexcept
                : m_value(value_)
            {}

            uint32_t value() const noexcept
            {
                return m_value;
            }

            uint32_t m_value;
        };

        /**
         * @brief Stack backtrace attached to exception.
         */
        struct stack : public tag<stack, mge::stacktrace>
        {
            /**
             * @brief Capture stack backtrace.
             *
             * @param s stack backtrace
             */
            stack(mge::stacktrace&& s)
                : m_value(std::move(s))
            {}

            const mge::stacktrace& value() const noexcept
            {
                return m_value;
            }

            mge::stacktrace m_value;
        };

        /**
         * @brief Message attached to exception.
         */
        struct message : public tag<message, std::string>
        {
            message() {}

            std::string_view value() const noexcept
            {
                return m_value;
            }

            std::string m_value;
        };

        /**
         * @brief Exception type name.
         */
        struct type_name : public tag<type_name, std::string>
        {
            /**
             * @brief Capture exception type name (subclass of mge::exception).
             *
             * @param name type name
             */
            type_name(std::string_view name)
                : m_value(name)
            {}

            const std::string& value() const noexcept
            {
                return m_value;
            }

            std::string m_value;
        };

        /**
         * @brief Function in which exception is thrown.
         */
        struct called_function : public tag<called_function, std::string_view>
        {
            /**
             * @brief Capture called function that raised the error.
             *
             * @param name called function
             */
            called_function(const std::string_view& name)
                : m_value(name)
            {}

            std::string_view value() const noexcept
            {
                return m_value;
            }

            std::string_view m_value;
        };

        struct cause;

        /**
         * @brief Construct empty exception.
         * @param loc source location
         */
        exception(
            const std::source_location& loc = std::source_location::current());

        /**
         * @brief Copy constructor.
         * @param ex copied exception
         */
        exception(const exception& ex);

        /**
         * @brief Move constructor.
         * @param ex moved exception
         */
        exception(exception&& ex);

        /**
         * Destructor.
         */
        virtual ~exception();

        /**
         * Assignment.
         * @return @c *this
         */
        exception& operator=(const exception&);

        /**
         * Move assignment.
         * @param e moved exception
         * @return @c *this
         */
        exception& operator=(exception&& e);

        /**
         * Overrides @c std::exception @c what function.
         * @return exception message
         */
        const char* what() const override;

        /**
         * Get current exception of this thread.
         * @return pointer to current exception or @c nullptr if there is none
         */
        static mge::exception* current_exception();

        /**
         * @brief Set information associated with tag type.
         *
         * @tparam Info info tag type
         * @param info information stored under the tag
         * @return  @c *this
         */
        template <typename Info> inline exception& set_info(const Info& info)
        {
            m_infos[std::type_index(typeid(typename Info::tag_type))] =
                info.value();
            return *this;
        }

        /**
         * @brief Set information associated with exception message.
         *
         * @tparam  exception::message
         * @param info info object containing message
         * @return @c *this
         */
        template <>
        exception& set_info<exception::message>(const exception::message& info)
        {
            m_raw_message = info.value();
            return *this;
        }

        /**
         * @brief Retrieve information stored under a tag type.
         *
         * @tparam Info  tag type
         * @return  the stored value
         */
        template <typename Info> inline auto get() const
        {
            auto it =
                m_infos.find(std::type_index(typeid(typename Info::tag_type)));
            std::optional<typename Info::value_type> result;
            if (it != m_infos.end()) {
                result = std::any_cast<typename Info::value_type>(it->second);
            }
            return result;
        }

        /**
         * @brief Get an exception details instance referring to this
         * exception.
         *
         * @return exception details for this exception
         */
        exception_details details() const noexcept
        {
            return exception_details(this);
        }

        /**
         * @brief Set exception information.
         * This is same as calling @c set_info.
         *
         * @tparam T type of appended value, is a @c tag type
         * @param value value to set
         * @return @c *this
         */
        template <class T>
        typename std::enable_if<std::is_base_of<tag_base, T>::value,
                                exception&>::type
        operator<<(const T& value)
        {
            set_info(value);
            return *this;
        }

        /**
         * @brief Append value to message.
         *
         * @tparam T type of appended value
         * @param value value to append
         * @return @c *this
         */
        template <class T>
        typename std::enable_if<!std::is_base_of<tag_base, T>::value,
                                exception&>::type
        operator<<(const T& value)
        {
            if (!m_raw_message_stream) {
                m_raw_message_stream = std::make_unique<std::stringstream>();
                if (!m_raw_message.empty()) {
                    (*m_raw_message_stream) << m_raw_message;
                    m_raw_message.clear();
                }
            }

            (*m_raw_message_stream) << value;

            return *this;
        }

    private:
        using exception_info_map = std::map<std::type_index, std::any>;
        exception_info_map m_infos;

    private:
        void copy_message_or_materialize(const exception& e);
        void materialize_message() const;

        mutable std::unique_ptr<std::stringstream> m_raw_message_stream;
        mutable std::string                        m_raw_message;
    };
} // namespace mge

template <>
struct fmt::formatter<mge::exception> : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::exception& ex, FormatContext& ctx) const
    {
        return fmt::formatter<std::string_view>::format(ex.what(), ctx);
    }
};

namespace mge {

    inline std::ostream& operator<<(std::ostream& os, const mge::exception& ex)
    {
        fmt::print(os, "{}", ex);
        return os;
    }

    /**
     * @brief Exception cause.
     */
    struct exception::cause : public tag<cause, mge::exception>
    {
        /**
         * @brief Capture causing exception.
         *
         * @param ex causing exception
         */
        cause(const mge::exception& ex)
            : m_value(ex)
        {}

        /**
         * @brief Capture causing exception.
         *
         * @param ex causing exception
         */
        cause(mge::exception&& ex)
            : m_value(std::move(ex))
        {}

        const mge::exception& value() const noexcept
        {
            return m_value;
        }

        mge::exception m_value;
    };

} // namespace mge

template <>
struct fmt::formatter<mge::exception::exception_details>
    : public fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const mge::exception::exception_details& ed,
                FormatContext&                           ctx) const
    {
        mge::exception::exception_details current_ed = ed;

        if (current_ed.ex() == nullptr) {
            fmt::format_to(ctx.out(), "No exception details available\n");
            return ctx.out();
        }

        while (current_ed.ex()) {
            if (current_ed.ex() != ed.ex()) {
                fmt::format_to(ctx.out(), "Caused by:");
            } else {
                fmt::format_to(ctx.out(), "Exception details:\n");
            }

            auto type = current_ed.ex()->get<mge::exception::type_name>();
            if (type) {
                fmt::format_to(ctx.out(), "Exception type: {}\n", type.value());
            } else {
                fmt::format_to(ctx.out(),
                               "Exception type: unknown mge::exception\n");
            }

            auto file = current_ed.ex()->get<mge::exception::source_file>();
            auto line = current_ed.ex()->get<mge::exception::source_line>();
            if (file && line) {
                fmt::format_to(ctx.out(),
                               "Exception location: {}:{}\n",
                               file.value(),
                               line.value());
            }

            auto function = current_ed.ex()->get<mge::exception::function>();
            if (function) {
                fmt::format_to(ctx.out(),
                               "Exception raising function: {}\n",
                               function.value());
            }
            auto called_function =
                current_ed.ex()->get<mge::exception::called_function>();
            if (called_function) {
                fmt::format_to(ctx.out(),
                               "Calling library/system function: {}\n",
                               called_function.value());
            }
            auto stack = current_ed.ex()->get<mge::exception::stack>();
            if (stack) {
                fmt::format_to(ctx.out(),
                               "Exception stack: {}\n",
                               stack.value());
            }
            fmt::format_to(ctx.out(),
                           "Exception message: {}\n",
                           current_ed.ex()->what());

            auto cause = current_ed.ex()->get<mge::exception::cause>();
            if (cause) {
                current_ed = cause->details();
            } else {
                current_ed = mge::exception::exception_details(nullptr);
            }
        }
        return ctx.out();
    }
};

namespace mge {

    MGECORE_EXPORT std::ostream&
    operator<<(std::ostream& os, const mge::exception::exception_details& ed);

/**
 * Throw exception instance.
 * @param ex exception type
 */
#define MGE_THROW(ex)                                                          \
    throw(ex().set_info(mge::exception::stack(mge::stacktrace()))              \
              .set_info(mge::exception::type_name(mge::type_name<ex>())))

/**
 * Throw exception and adds a cause.
 * @param ex exception type
 * @param causing_exception exception causing this exception
 */
#define MGE_THROW_WITH_CAUSE(ex, causing_exception)                            \
    throw ex()                                                                 \
        .set_info(mge::exception::stack(mge::stacktrace()))                    \
        .set_info(mge::exception::type_name(mge::type_name<ex>()))             \
        .set_info(mge::exception::cause(causing_exception))

/**
 * @def MGE_CALLED_FUNCTION
 * @brief Helper to add called function to exception information.
 * @param X name of called function
 * This helper is usually used to attach additional information
 * to the exception, e.g. in case of system or foreign API errors.
 * @code
 * MGE_THROW(mge::exception) << MGE_CALLED_FUNCTION(fopen);
 * @endcode
 */
#define MGE_CALLED_FUNCTION(X) ::mge::exception::called_function(#X)

    /**
     * @brief Re-throws the current exception.
     *
     * This function does not return.
     */
    [[noreturn]] inline void rethrow()
    {
        throw;
    }

} // namespace mge
// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/stacktrace.hpp"
#include <any>
#include <exception>
#include <optional>
#include <sstream>
#include <string_view>
#include <typeindex>
#include <typeinfo>

namespace mge {

    /**
     * @brief An exception.
     *
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
            exception_details(const mge::exception *ex) noexcept : m_ex(ex) {}
            inline const mge::exception *ex() const noexcept { return m_ex; }

        private:
            const mge::exception *m_ex;
        };

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
        template <typename Tag, typename Value> struct tag
        {
            using tag_type   = Tag;   //!< Tag type.
            using value_type = Value; //!< Value type of value stored under tag.
        };

        /**
         * @brief Source file name attached to exception.
         */
        struct source_file : public tag<source_file, std::string_view>
        {
            source_file(std::string_view value_) noexcept : m_value(value_) {}

            std::string_view value() const noexcept { return m_value; }

            std::string_view m_value;
        };

        /**
         * @brief Function name attached to exception.
         */
        struct function : public tag<function, std::string_view>
        {
            function(std::string_view value_) noexcept : m_value(value_) {}

            std::string_view value() const noexcept { return m_value; }

            std::string_view m_value;
        };

        /**
         * @brief Source file line number attached to exception.
         */
        struct source_line : public tag<source_line, uint32_t>
        {
            source_line(uint32_t value_) noexcept : m_value(value_) {}

            uint32_t value() const noexcept { return m_value; }

            uint32_t m_value;
        };

        /**
         * @brief Stack backtrace attached to exception.
         */
        struct stack : public tag<stack, mge::stacktrace>
        {
            stack(mge::stacktrace &&s) : m_value(std::move(s)) {}

            const mge::stacktrace &value() const noexcept { return m_value; }

            mge::stacktrace m_value;
        };

        /**
         * @brief Message attached to exception.
         */
        struct message : public tag<message, std::string>
        {
            message() {}

            const std::string &value() const noexcept { return m_value; }

            std::string m_value;
        };

        /**
         * @brief Exception type name.
         */
        struct type_name : public tag<type_name, std::string>
        {
            type_name(const std::string &name) : m_value(name) {}

            const std::string &value() const noexcept { return m_value; }

            std::string m_value;
        };

        /**
         * @brief Function in which exception is thrown.
         */
        struct called_function : public tag<called_function, std::string_view>
        {
            called_function(std::string_view name) : m_value(name) {}

            std::string_view value() const noexcept { return m_value; }

            std::string_view m_value;
        };

        struct cause;

        /**
         * @brief Construct empty exception.
         */
        exception();

        /**
         * @brief Copy constructor.
         * @param ex copied exception
         */
        exception(const exception &ex);

        /**
         * @brief Move constructor.
         * @param ex moved exception
         */
        exception(exception &&ex);

        /**
         * Destructor.
         */
        virtual ~exception();

        /**
         * Assignment.
         * @return @c *this
         */
        exception &operator=(const exception &);

        /**
         * Move assignment.
         * @param e moved exception
         * @return @c *this
         */
        exception &operator=(exception &&e);

        /**
         * Overrides @c std::exception @c what function.
         * @return exception message
         */
        const char *what() const override;

        /**
         * Get current exception of this thread.
         * @return pointer to current exception or @c nullptr if there is none
         */
        static mge::exception *current_exception();

        /**
         * @brief Set information associated with tag type.
         *
         * @tparam Info info tag type
         * @param info information stored under the tag
         * @return  @c *this
         */
        template <typename Info> exception &set_info(const Info &info)
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
        exception &set_info<exception::message>(const exception::message &info)
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
            const auto message_key =
                std::type_index(typeid(typename exception::message::tag_type));
            auto it =
                m_infos.find(std::type_index(typeid(typename Info::tag_type)));
            std::optional<Info::value_type> result;
            if (it != m_infos.end()) {
                result = std::any_cast<Info::value_type>(it->second);
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
         * @brief Append value to message.
         *
         * @tparam T type of appended value
         * @param value value to append
         * @return @c *this
         */
        template <typename T> exception &operator<<(const T &value)
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
        void copy_message_or_materialize(const exception &e);
        void materialize_message() const;

        mutable std::unique_ptr<std::stringstream> m_raw_message_stream;
        mutable std::string                        m_raw_message;
    };

    /**
     * @brief Exception cause.
     */
    struct exception::cause : public tag<cause, mge::exception>
    {
        cause(const mge::exception &ex) : m_value(ex) {}

        cause(mge::exception &&ex) : m_value(std::move(ex)) {}

        const mge::exception &value() const noexcept { return m_value; }

        mge::exception m_value;
    };

    /**
     * @brief Print exception message.
     *
     * @param os output stream
     * @param ex exception
     * @return @c os
     */
    MGECORE_EXPORT std::ostream &operator<<(std::ostream &os, exception &ex);

    /**
     * @brief Print exception details.
     *
     * @param os output stream
     * @param details wrapped exception
     * @return @c os
     */
    MGECORE_EXPORT std::ostream &
    operator<<(std::ostream &os, exception::exception_details &details);
    /**
     * @brief Re-throws the current exception.
     *
     * This function does not return.
     */
    [[noreturn]] inline void rethrow() { throw; }

} // namespace mge
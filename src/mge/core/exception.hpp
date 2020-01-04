// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/format_string.hpp"
#include "mge/core/type_name.hpp"

#include <boost/any.hpp>
#include <boost/optional.hpp>

#include <sstream>
#include <tuple>
#include <map>
#include <typeinfo>
#include <typeindex>

/**
 * @file mge/core/exception.hpp
 * @brief Exception class.
 */
namespace mge {

    class exception;

    /**
     * @brief Base exception class.
     */
    class MGE_CORE_EXPORT exception
            : virtual public std::exception
    {
    public:
        /**
         * @internal
         * Helper class for detailed exception information.
         */
        struct exception_details
        {
        public:
            exception_details(const mge::exception* ex) noexcept
                :m_ex(ex)
            {}
            inline const mge::exception* ex() const noexcept { return m_ex; }
        private:
            const mge::exception *m_ex;
        };


        /**
         * @brief Exception value tag type.
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
        template <typename Tag, typename Value>
        struct tag
        {
            typedef Tag     tag_type;    //!< Tag type.
            typedef Value   value_type;  //!< Value type of value stored under tag.
        };

        /**
         * @brief Source file name attached to exception.
         */
        struct source_file : public tag<source_file, const char *>
        {
            source_file(const char *value_) noexcept
                :m_value(value_)
            {}

            const char *value() const noexcept
            {
                return m_value;
            }

            const char *m_value;
        };

        /**
         * @brief Function name attached to exception.
         */
        struct function : public tag<function, const char *>
        {
            function(const char *value_) noexcept
                :m_value(value_)
            {}

            const char *value() const noexcept
            {
                return m_value;
            }

            const char *m_value;
        };

        /**
         * @brief Source file line number attached to exception.
         */
        struct source_line : public tag<source_line, int>
        {
            source_line(int value_) noexcept
                :m_value(value_)
            {}

            int value() const noexcept
            {
                return m_value;
            }

            int m_value;
        };
        /**
         * @brief Stack backtrace attached to exception.
         */
        struct stack : public tag<stack, mge::stacktrace>
        {
            stack(mge::stacktrace&& s)
                :m_value(std::move(s))
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
            message()
            {}

            const std::string& value() const noexcept
            {
                return m_value;
            }

            std::string m_value;
        };

        struct type_name : public tag<type_name, std::string>
        {
            type_name(const std::string& name)
                :m_value(name)
            {}

            const std::string& value() const noexcept
            {
                return m_value;
            }

            std::string m_value;
        };

        struct called_function : public tag<called_function, std::string>
        {
            called_function(const std::string& name)
                :m_value(name)
            {}

            const std::string& value() const noexcept
            {
                return m_value;
            }

            std::string m_value;
        };


        struct cause;

        /**
         * Constructor.
         */
        exception();
        /**
         * Copy constructor.
         * @param e copied exception
         */
        exception(const exception& e);
        /**
         * Move constructor.
         * @param e moved exception
         */
        exception(exception&& e);

        /**
         * Destructor.
         */
        virtual ~exception();

        /**
         * Assignment.
         * @return @c *this
         */
        exception& operator =(const exception&);

        /**
         * Move assignment.
         * @param e moved exception
         * @return @c *this
         */
        exception& operator =(exception&& e);

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

        template <typename Info>
        exception& set_info(const Info& info)
        {
            const auto message_key = std::type_index(typeid(typename exception::message::tag_type));
            m_infos[std::type_index(typeid(typename Info::tag_type))]
                    = info.value();
            return *this;
        }

        template<>
        exception& set_info<exception::message>(const exception::message& info)
        {
            m_raw_message = info.value();
            return *this;
        }

        template <typename Info>
        inline auto get() const
        {
            const auto message_key = std::type_index(typeid(typename exception::message::tag_type));
            auto it = m_infos.find(std::type_index(typeid(typename Info::tag_type)));
            boost::optional<Info::value_type> result;
            if (it != m_infos.end()) {
                result = boost::any_cast<Info::value_type>(it->second);
            }
            return result;
        }

        template<>
        inline auto get<exception::message>() const
        {
            materialize_message();
            return m_raw_message;
        }

        exception_details details() const noexcept
        {
            return exception_details(this);
        }

        template <typename T>
        exception& operator <<(const T& value)
        {
            if(!m_raw_message_stream) {
                m_raw_message_stream = std::make_unique<std::stringstream>();
                if(!m_raw_message.empty()) {
                    (*m_raw_message_stream) << m_raw_message;
                    m_raw_message.clear();
                }
            }

            (*m_raw_message_stream) << value;

            return *this;
        }


    private:
        typedef std::map<std::type_index, boost::any> exception_info_map_t;
        exception_info_map_t m_infos;
    private:
        void copy_message_or_materialize(const exception& e);
        void materialize_message() const;

        mutable std::unique_ptr<std::stringstream> m_raw_message_stream;
        mutable std::string m_raw_message;
    };


    struct exception::cause : public tag<cause, mge::exception>
    {
        cause(const mge::exception& ex)
            :m_value(ex)
        {}

        cause(mge::exception&& ex)
            :m_value(std::move(ex))
        {}

        const mge::exception& value() const noexcept
        {
            return m_value;
        }

        mge::exception m_value;
    };

/**
 * Throw exception instance.
 * @param ex exception type
 */
#define MGE_THROW(ex)                                                     \
    throw ex().set_info(mge::exception::source_file(__FILE__))            \
              .set_info(mge::exception::source_line(__LINE__))            \
              .set_info(mge::exception::function(MGE_FUNCTION_SIGNATURE)) \
              .set_info(mge::exception::stack(mge::stacktrace()))         \
              .set_info(mge::exception::type_name(mge::type_name<ex>()))

#define MGE_THROW_WITH_CAUSE(ex, causing_exception)                       \
    throw ex().set_info(mge::exception::source_file(__FILE__))            \
              .set_info(mge::exception::source_line(__LINE__))            \
              .set_info(mge::exception::function(MGE_FUNCTION_SIGNATURE)) \
              .set_info(mge::exception::stack(mge::stacktrace()))         \
              .set_info(mge::exception::type_name(mge::type_name<ex>()))  \
              .set_info(mge::exception::cause(causing_exception))

#define MGE_CALLED_FUNCTION(X) ::mge::exception::called_function(#X)


    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os, const mge::exception& ex);
    MGE_CORE_EXPORT std::ostream& operator <<(std::ostream& os, const mge::exception::exception_details& d);

    /**
     * Re-throws current exception. 
     */
    [[noreturn]] inline void rethrow()
    {
        throw;
    }
}



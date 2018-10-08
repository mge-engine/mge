// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/format_string.hpp"

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

    /**
     * @brief Base exception class.
     */
    class MGE_CORE_EXPORT exception
            : virtual public std::exception
    {
    public:
        template <typename Tag, typename Value>
        struct tag
        {
            typedef Tag     tag_type;
            typedef Value   value_type;
        };

        struct source_file : public tag<source_file, const char *>
        {
            source_file(const char *value_)
                :value(value_)
            {}

            const char *value;
        };

        struct function : public tag<function, const char *>
        {
            function(const char *value_)
                :value(value_)
            {}

            const char *value;
        };

        struct source_line : public tag<source_line, int>
        {
            source_line(int value_)
                :value(value_)
            {}

            int value;
        };

        struct stack : public tag<stack, mge::stacktrace>
        {
            stack(mge::stacktrace&& s)
                :value(std::move(s))
            {}

            mge::stacktrace value;
        };


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
        exception& operator =(exception&&);

        /**
         * Overrides @c std::exception @c what function.
         * @return exception message
         */
        const char *what() const;

        template <typename Info>
        exception& operator <<(const Info& info)
        {
            m_infos[std::type_index(typeid(typename Info::tag_type))]
                    = info.value;
            return *this;
        }

        template <typename Info>
        inline auto get() const
        {
            auto it = m_infos.find(std::type_index(typeid(typename Info::tag_type)));
            boost::optional<Info::value_type> result;
            if (it != m_infos.end()) {
                result = boost::any_cast<Info::value_type>(it->second);
            }
            return result;
        }

    private:
        typedef std::map<std::type_index, boost::any> exception_info_map_t;
        exception_info_map_t m_infos;
    };


#define MGE_THROW(ex, ...)                                          \
    throw (ex) << mge::exception::source_file(__FILE__)             \
               << mge::exception::source_line(__LINE__)             \
               << mge::exception::function(MGE_FUNCTION_SIGNATURE)  \
               << mge::exception::stack(mge::stacktrace())

//    << mge::excinfo_source_file(__FILE__)                \
//               << mge::excinfo_source_line(__LINE__)                \
//               << mge::excinfo_stack(mge::stacktrace())             \
//               << mge::excinfo_function(MGE_FUNCTION_SIGNATURE)     \
//               << mge::exception_message(__VA_ARGS__)



}

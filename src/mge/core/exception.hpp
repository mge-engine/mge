// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/stacktrace.hpp"
#include "mge/core/format_string.hpp"
#include <boost/any.hpp>
#include <sstream>
#include <tuple>

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

//        /**
//         * Extract info field.
//         * @return extracted info field
//         */
//        template <typename EI>
//        auto info() const
//        {
//            return boost::get_error_info<EI>(*this);
//        }

        /**
         * Overrides @c std::exception @c what function.
         * @return exception message
         */
        const char *what() const;
    };

//    /**
//     * @brief Exception info for called function.
//     *
//     * E.g. when throwing exception
//     * because of @c fopen has failed, this would be the
//     * string "fopen".
//     */
//    typedef boost::errinfo_api_function excinfo_api_function;
//    /**
//     * @brief
//     * Exception info for system error code, i.e. @c errno
//     * or @c GetLastError() results.
//     */
//    typedef boost::errinfo_errno excinfo_sysrc;

//    /**
//     * @brief Exception info for source code file of exception throw
//     * location.
//     */
//    typedef boost::throw_file excinfo_source_file;

//    /**
//     * @brief Exception info for source code line of exception throw
//     * location.
//     */
//    typedef boost::throw_line excinfo_source_line;

//    /**
//     * @brief Exception info for function of exception throw location.
//     */
//    typedef boost::throw_function excinfo_function;

//    /**
//     * @brief Helper structure for stacktrace exception info.
//     */
//    struct stacktrace_excinfo_tag {};

//    /**
//     * @brief Exception info for stack backtrace.
//     */
//    typedef error_info<stacktrace_excinfo_tag, stacktrace> excinfo_stack;

//    /**
//     * @brief Helper tag structure for exception description ("long text")
//     */
//    struct desc_excinfo_tag {};

//    /**
//     * @brief Exception info for long text.
//     */
//    typedef error_info<desc_excinfo_tag, std::string> excinfo_desc;

//    /**
//     * Construct exception message from arguments.
//     */
//    template <typename... Args>
//    mge::excinfo_desc exception_message(Args ... args)
//    {
//        return mge::excinfo_desc(format_string(args...));
//    }


#define MGE_THROW(ex, ...)                                          \
    throw (ex)


//    << mge::excinfo_source_file(__FILE__)                \
//               << mge::excinfo_source_line(__LINE__)                \
//               << mge::excinfo_stack(mge::stacktrace())             \
//               << mge::excinfo_function(MGE_FUNCTION_SIGNATURE)     \
//               << mge::exception_message(__VA_ARGS__)



}

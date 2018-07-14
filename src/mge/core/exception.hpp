// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#include "mge/core/stacktrace.hpp"

#include <boost/exception/all.hpp>
#include <boost/exception/error_info.hpp>

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
            ,  virtual public boost::exception
    {
    public:
        /**
         * Constructor.
         */
        exception();
        exception(const exception& e);
        exception(exception&& e);
        virtual ~exception();

        /**
         * Assignment.
         * @return
         */
        exception& operator =(const exception&);

        template <typename EI>
        auto info() const
        {
            return boost::get_error_info<EI>(*this);
        }

        /**
         * Overrides @c std::exception @c what function.
         * @return exception message
         */
        const char *what() const;
    };

    /**
     * @brief Exception info for called function.
     *
     * E.g. when throwing exception
     * because of @c fopen has failed, this would be the
     * string "fopen".
     */
    typedef boost::errinfo_api_function excinfo_api_function;
    /**
     * @brief
     * Exception info for system error code, i.e. @c errno
     * or @c GetLastError() results.
     */
    typedef boost::errinfo_errno excinfo_sysrc;

    /**
     * @brief Exception info for source code file of exception throw
     * location.
     */
    typedef boost::throw_file excinfo_source_file;

    /**
     * @brief Exception info for source code line of exception throw
     * location.
     */
    typedef boost::throw_line excinfo_source_line;

    /**
     * @brief Exception info for function of exception throw location.
     */
    typedef boost::throw_function excinfo_function;

    /**
     * @brief Helper structure for stacktrace exception info.
     */
    struct stacktrace_excinfo_tag {};

    /**
     * @brief Exception info for stack backtrace.
     */
    typedef boost::error_info<stacktrace_excinfo_tag, stacktrace> excinfo_stack;

    /**
     * @brief Helper tag structure for exception description ("long text")
     */
    struct desc_excinfo_tag {};

    /**
     * @brief Exception info for long text.
     */
    typedef boost::error_info<desc_excinfo_tag, std::string> excinfo_desc;


#define MGE_THROW(ex)                                               \
    throw (ex) << mge::excinfo_source_file(__FILE__)                \
               << mge::excinfo_source_line(__LINE__)                \
               << mge::excinfo_stack(mge::stacktrace())             \
               << mge::excinfo_function(MGE_FUNCTION_SIGNATURE)



}

// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/string_pool.hpp"

#ifdef MGE_OS_WINDOWS
#    include <windows.h>
#endif

#include <iosfwd>
#include <mutex>
#include <thread>

/**
 * @file mge/core/stacktrace.hpp
 * @brief Stack trace class.
 */

namespace mge {

    class thread;

    /**
     * @brief A stack trace.
     *
     * A strack trace contains a number of elements
     * where each element represents a stack frame in call graph.
     */
    class MGE_CORE_EXPORT stacktrace
    {
    public:
        /**
         * @brief Element of the stack trace.
         */
        struct element
        {
            /**
             * Constructor.
             * @param address_  frame address
             * @param function_ function
             * @param file_     source file
             * @param line_     line in source file
             * @param module_   module name
             */
            inline element(void *address_, const char *function_,
                           const char *file_, int line_, const char *module_)
                : address(address_), function(function_), file(file_),
                  line(line_), module(module_)
            {}

            /**
             * Copy constructor
             * @param e element to copy
             */
            inline element(const element &e) = default;

            void *      address;
            const char *function;
            const char *file;
            int         line;
            const char *module;
        };

        using container_type         = std::vector<element>;
        using value_type             = container_type::value_type;
        using allocator_type         = container_type::allocator_type;
        using size_type              = container_type::size_type;
        using difference_type        = container_type::difference_type;
        using reference              = container_type::reference;
        using const_reference        = container_type::const_reference;
        using pointer                = container_type::pointer;
        using const_pointer          = container_type::const_pointer;
        using iterator               = container_type::iterator;
        using const_iterator         = container_type::const_iterator;
        using reverse_iterator       = container_type::reverse_iterator;
        using const_reverse_iterator = container_type::const_reverse_iterator;

        /**
         * Creates a stack backtrace of the current call site.
         */
        stacktrace();
        /**
         * Creates a stacktrace of a thread.
         * @param t thread to capture
         */
        stacktrace(const std::thread &t);

        /**
         * Create stacktrace of a thread.
         * @param t thread to capture
         */
        stacktrace(const thread &t);

        /**
         * Copy constructor.
         * @param s copied stack trace
         */
        stacktrace(const stacktrace &s);

        /**
         * Move constructor.
         * @param s moved stack trace
         */
        stacktrace(stacktrace &&s);

        /**
         * Assignment operator.
         * @param s assigned stack trace
         * @return @c *this
         */
        stacktrace &operator=(const stacktrace &s);

        /**
         * Destructor.
         */
        ~stacktrace();

        /**
         * Comparision operator.
         * @param s stack trace to compare
         * @return @c true if all frames are equal
         */
        bool operator==(const stacktrace &s) const;

        /**
         * Comparision operator.
         * @param s stack trace to compare
         * @return @c true the frames are unequal
         */
        bool operator!=(const stacktrace &s) const;

        /**
         * Returns iterator to first element.
         * @return iterator to first element
         */
        const_iterator begin() const;

        /**
         * Returns iterator that points past last element.
         * @return pointer past last element
         */
        const_iterator end() const;

        /**
         * Get number of entries in stack trace.
         * @return number of stack trace elements
         */
        size_type size() const;

    private:
        void               fill();
        void               fill(const std::thread &t);
        static const char *intern_str(const char *str);
#ifdef MGE_OS_WINDOWS
        void fill(HANDLE thread_handle, CONTEXT *context);
#endif
        container_type m_elements;

        static string_pool s_strings;
        static std::mutex  s_strings_lock;
    };

    /**
     * Print a stack backtrace.
     * @param os output stream
     * @param s stack backtrace
     * @return @c s
     */
    MGE_CORE_EXPORT std::ostream &operator<<(std::ostream &    os,
                                             const stacktrace &s);
} // namespace mge

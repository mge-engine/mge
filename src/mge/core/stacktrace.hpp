// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file */
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/string_pool.hpp"
#include <string>
#include <string_view>
#include <vector>
namespace mge {

    /**
     * @brief A stack backtrace.
     *
     */
    class MGECORE_EXPORT stacktrace
    {
    public:
        /**
         * @brief A frame on the stack backtrace.
         */
        class MGECORE_EXPORT frame
        {
        public:
            /**
             * @brief Construct a new frame object
             *
             * @param address   address (program counter)
             * @param module    module name (executable/shared library)
             * @param name      name of frame (function)
             * @param file      source code file
             * @param line      source code line
             */
            frame(const void*      address,
                  std::string_view module,
                  std::string_view name,
                  std::string_view file,
                  uint32_t         line);
            /**
             * @brief Copy constructor.
             * @param f copied frame
             */
            frame(const frame& f) = default;

            ~frame() = default;

            /**
             * @brief Assignment.
             *
             * @return f assigned frame
             */
            frame& operator=(const frame& f) = default;

            /**
             * @brief Get frame address.
             *
             * @return frame address
             */
            const void* address() const noexcept { return m_address; }
            /**
             * @brief Frame name (method or function name)
             *
             * @return method or function name
             */
            std::string_view name() const noexcept { return m_name; }

            /**
             * @brief Source file name.
             *
             * @return source file name
             */
            std::string_view source_file() const noexcept
            {
                return m_source_file;
            }

            /**
             * @brief Source line number.
             *
             * @return source line number
             */
            uint32_t source_line() const noexcept { return m_source_line; }

            /**
             * @brief Module name (executable or library)
             *
             * @return module name
             */
            std::string_view module() const noexcept { return m_module; }

        private:
            const void*      m_address;
            std::string_view m_name;
            std::string_view m_source_file;
            std::string_view m_module;
            uint32_t         m_source_line;
        };

    private:
        using frame_vector = std::vector<frame>;

    public:
        /// stack trace size
        using size_type = frame_vector::size_type;
        /// iterator on frames
        using const_iterator = frame_vector::const_iterator;
        /// iterator on frames
        using const_reverse_iterator = frame_vector::const_reverse_iterator;

        /**
         * @brief Construct stacktrace of current thread.
         */
        stacktrace();
        /**
         * @brief Copy constructor.
         *
         * @param s copied stack trace
         */
        stacktrace(const stacktrace& s);
        /**
         * @brief Destructor.
         *
         */
        ~stacktrace() = default;

        /**
         * @brief Assignment
         *
         * @param s assigned stack trace
         * @return @c *this
         */
        stacktrace& operator=(const stacktrace& s);

        /**
         * @brief Number of frames.
         *
         * @return number of frames
         */
        size_type size() const;
        /**
         * @brief Begin of frames.
         *
         * @return begin of frames
         */
        const_iterator begin() const { return m_frames.begin(); }
        /**
         * @brief Reverse begin of frames.
         *
         * @return reverse begin of frames
         */
        const_reverse_iterator rbegin() const { return m_frames.rbegin(); }
        /**
         * @brief End of frames.
         *
         * @return end of frames
         */
        const_iterator end() const { return m_frames.end(); }

        /**
         * @brief Reverse end  of frames.
         *
         * @return reverse end of frames
         */
        const_iterator rend() const { return m_frames.end(); }

        /**
         * @brief Comparison.
         *
         * @param s compared stack trace
         * @return true if adresses match
         */
        bool operator==(const stacktrace& s) const;
        /**
         * @brief Comparison.
         *
         * @param s compared stack trace
         * @return true if adresses do not match
         */
        bool operator!=(const stacktrace& s) const;

    private:
        frame_vector m_frames;
        string_pool  m_strings;
    };

    /**
     * @brief Dump stack into stream.
     *
     * @param os output stream
     * @param s stack backtrace
     * @return @c os
     */
    MGECORE_EXPORT std::ostream& operator<<(std::ostream&     os,
                                            const stacktrace& s);

} // namespace mge
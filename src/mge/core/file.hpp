// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
/**
 * @file   file.hpp
 * @brief  File handling.
 */

#include "mge/core/file_access.hpp"
#include "mge/core/path.hpp"

namespace mge {
    /**
     * Handle to a file (or directory).
     */
    class MGE_CORE_EXPORT file
    {
    public:
        file(const file_access_ref& access);

        /**
         * Create file.
         * @param path
         */
        file(const char *p);

        /**
         * Create file.
         * @param path
         */
        file(const std::string& p);

        /**
         * Create file from path.
         * @param path path
         */
        file(const path& p);

        /**
         * Destructor.
         */
        ~file() = default;

        /**
         * Constructor.
         * @param path file path
         * @param name file name
         */
        file(const std::string& p, const std::string& name);

        /**
         * Get file path.
         * @return file path
         */
        const path& path() const;

        /**
         * Check if file exists.
         * @return @c true if exists
         */
        bool exists() const;

        /**
         * Check if file is a directory.
         * @return @c true if it is a directory
         */
        bool is_directory() const;

        /**
         * Check whether file is a real file
         * @return @c true if it is a file
         */
        bool is_file() const;

        /**
         * Check whether the file is a real file system file. Some operations
         * can only done on real file system files.
         * @return @c true if the file is a real file system file
         */
        bool is_system_file() const;

        /**
         * Create the directory described by this file.
         */
        void mkdir();

        /**
         * Remove directory described by this file.
         */
        void rmdir();

        /**
         * List contents of directory.
         * @param files files filled
         */
        std::vector<file> list() const;

        /**
         * Opens file for input.
         * @return an input stream reference to opened file
         */
        input_stream_ref open_for_input() const;

        /**
         * @brief Opens a file for output.
         *
         * @return output_stream_ref
         */
        output_stream_ref open_for_output() const;

        /**
         * Return file size.
         *
         * @return file size, 0 if the file isn't a regular file
         */
        size_t size() const;
    private:
        file_access_ref m_access;
    };

    /**
     * Stream output operator. Prints the file name.
     * @param os stream
     * @param f file
     * @return @c os
     */
    MGE_CORE_EXPORT std::ostream&
    operator <<(std::ostream& os, const file& f);

}

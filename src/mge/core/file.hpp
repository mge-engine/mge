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
        file(const char *path);

        /**
         * Create file.
         * @param path
         */
        file(const std::string& path);

        /**
         * Create file from path.
         * @param path path
         */
        file(const path& path);

        /**
         * Destructor.
         */
        ~file() = default;

        /**
         * Constructor.
         * @param path file path
         * @param name file name
         */
        file(const std::string& path, const std::string& name);

        /**
         * Get the extension of the file.
         * @return extension string, may be empty if file has no extension
         */
        std::string file_extension() const;

        /**
         * Get path of file.
         * @return path
         */
        std::string file_path() const;

        /**
         * Get file name (without directory).
         * @return file name
         */
        std::string file_name() const;

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
        void list(std::vector<file>& files);

        /**
         * Opens file for input.
         * @return an input stream reference to opened file
         */
        input_stream_ref open_for_input() const;

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

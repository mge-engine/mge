// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/file_not_found.hpp"
#include "mge/core/filesystem_error.hpp"
#include "mge/core/path.hpp"
#include "mge/core/types.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/input_stream.hpp"

#include <string>
#include <vector>

namespace mge {

    class file;

    /// @class file_access_ref
    /// @brief Shared pointer to file access.
    MGE_DECLARE_REF(file_access);

    /**
     * File access handler. Handles file access of one file, identified by a path.
     */
    class MGE_CORE_EXPORT file_access : public noncopyable
    {
    public:
        /**
         * Constructor.
         * @param path path of handled file
         */
        file_access(const std::string& path);

        /**
         * Destructor.
         */
        virtual ~file_access() = default;

        /**
         * Return whether a file exists.
         * @return @c true if file just now exists
         */
        virtual bool exists() const = 0;

        /**
         * Return whether file is a normal file.
         * @return @c true if it is a file
         */
        virtual bool is_file() const = 0;

        /**
         * Return whether file is a directory.
         * @return @c true if it is a directory
         */
        virtual bool is_directory() const = 0;

        /**
         * Return whether file is a real file system file.
         * @return @c true if a real file
         */
        virtual bool is_system_file() const = 0;

        /**
         * Create directory.
         */
        virtual void mkdir() = 0;

        /**
         * Remove directory.
         */
        virtual void rmdir() = 0;

        /**
         * List directory contents.
         * @param files list of files filled
         */
        virtual void list(std::vector<file>& files) = 0;

        /**
         * Get the path handled by this access object.
         * @return path
         */
        const mge::path& path() const;

        /**
         * Opens the file for input.
         * @return handle to opened stream
         */
        virtual input_stream_ref open_for_input() const = 0;

    protected:
        /// Constructor to be used if path cannot be determined.
        file_access() = default;
    protected:
        mge::path m_path;
    };
}

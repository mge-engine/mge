// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
/**
 * @file   archive.hpp
 * @brief  Archive file.
 */

#include "mge/core/path.hpp"
#include "mge/core/file.hpp"
#include "mge/core/open_mode.hpp"


namespace mge {

    MGE_DECLARE_REF(archive_access);

    /**
     * Handle to an archive.
     */
    class MGE_CORE_EXPORT archive
    {
    public:
        /**
         * @brief Open mode of an archive.
         */
        using open_mode = mge::open_mode;
        /**
         * @brief Archive entry.
         */
        class MGE_CORE_EXPORT entry
        {
        public:
            /**
             * @brief Construct a new entry.
             *
             * @param path          entry path
             * @param is_directory  whether entry is a directory
             * @param size          size of the entry
             * @param index         index of entry in archive
             */
            entry(const path& path,
                  bool is_directory,
                  std::streamsize size,
                  uint64_t index);
            /**
             * @brief Copy constructor.
             *
             * @param e copied entry
             */
            entry(const entry& e);
            /**
             * @brief Move constructor.
             *
             * @param e moved entry
             */
            entry(entry&& e);

            /**
             * @brief Destructor.
             */
            ~entry() = default;

            /**
             * @brief Assignment.
             *
             * @param e assigned entry
             * @return @c *this
             */
            entry& operator=(const entry& e);
            /**
             * @brief Move assignment.
             *
             * @param e moved entry
             * @return @c *this
             */
            entry& operator=(entry&& e);

            /**
             * @brief Get path of entry.
             *
             * @return entry path
             */
            const mge::path& path() const;

            /**
             * @brief Check if entry is directory.
             *
             * @return @c true if the entry is a directory
             */
            bool is_directory() const noexcept
            {
                return m_directory;
            }

            /**
             * @brief Entry size.
             *
             * @return size of entry in bytes (if file)
             */
            std::streamsize size() const
            {
                return m_size;
            }

            /**
             * @brief Open stream for entry data.
             *
             * @return stream for reading entry data
             */
            input_stream_ref open();
        private:
            friend class archive;
            void set_access(const archive_access_ref& access);

            mge::path          m_path;
            std::streamsize    m_size;
            archive_access_ref m_access;
            uint64_t           m_index;
            bool               m_directory;
        };

        /**
         * @brief List of entries.
         */
        using archive_entries = std::vector<entry>;

        /**
         * @brief Construct a new archive object.
         *
         * @param path path to archive
         * @param mode open mode
         */
        archive(const path& path, open_mode mode = open_mode::READ);
        /**
         * @brief Construct a new archive object
         *
         * @param file archive file
         * @param mode open mode
         */
        archive(const file& file, open_mode mode = open_mode::READ);
        /**
         * @brief Construct a new archive object
         *
         * @param path path to archive file
         * @param mode open mode
         */
        archive(const char* path, open_mode mode = open_mode::READ);
        /**
         * @brief Construct a new archive object
         *
         * @param path path to archive file
         * @param mode open mode
         */
        archive(const std::string& path, open_mode mode = open_mode::READ);

        /**
         * @brief Destructor.
         *
         */
        ~archive();

        /**
         * @brief Access archive entries.
         *
         * @return flat list of archive entries
         */
        const archive_entries& entries() const
        {
            return m_entries;
        }

        /**
         * @brief Find archive entry by path
         *
         * @param p entry path
         * @return entry that has the path, @c no_such_element is thrown if
         *   no entry is found
         */
        const entry& find_entry(const mge::path& p) const;

    private:
        void open();

        file               m_file;
        open_mode          m_open_mode;
        archive_access_ref m_access;
        archive_entries    m_entries;
    };
}



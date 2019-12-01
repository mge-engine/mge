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
#include "mge/core/archive_access.hpp"

namespace mge {

    /**
     * Handle to an archive.
     */
    class MGE_CORE_EXPORT archive
    {
    public:
        using open_mode = mge::open_mode;
        class entry
        {
        public:
            entry(const path& p, bool dir);
            entry(const entry& e);
            entry(entry&& e);
            
            ~entry() = default;

            entry& operator=(const entry& e);
            entry& operator=(entry&& e);

            const mge::path& path() const;

            bool is_directory() const noexcept
            {
                return m_directory; 
            }
        private:
            mge::path m_path;
            bool      m_directory;
        };

        using archive_entries = std::vector<entry>;

        archive(const path& path, open_mode mode = open_mode::READ);
        archive(const file& file, open_mode mode = open_mode::READ);
        archive(const char* path, open_mode mode = open_mode::READ);
        archive(const std::string& path, open_mode mode = open_mode::READ);
        ~archive();

        const archive_entries& entries() const
        {
            return m_entries;
        }

    private:
        void open();
        
        file               m_file;
        open_mode          m_open_mode;
        archive_access_ref m_access;
        archive_entries    m_entries;
    };
}



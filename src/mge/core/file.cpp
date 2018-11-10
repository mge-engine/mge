// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file.hpp"
#include "mge/core/file_access_factory.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    class file_access_provider
    {
    public:
        file_access_provider() = default;
        ~file_access_provider() = default;

        file_access_ref create_file_access(const std::string& path)
        {
            if (!m_system_factory) {
                m_system_factory = file_access_factory::create("system");
                if (!m_system_factory) {
                    MGE_THROW(illegal_state(),
                              "System file access factory not initialized");
                }
            }

            file_access_ref result = m_system_factory->create_file_access(path);
            return result;
        }

        file_access_ref create_file_access(const std::string& path,
                                           const std::string& name)
        {
            if (!m_system_factory) {
                m_system_factory = file_access_factory::create("system");
                if (!m_system_factory) {
                    MGE_THROW(illegal_state(),
                              "System file access factory not initialized");
                }
            }

            return m_system_factory->create_file_access(path,
                                                        name);
        }

        file_access_factory_ref m_system_factory;
    };

    static singleton<file_access_provider> s_file_access_provider;

    file::file(const file_access_ref& access)
                    : m_access(access)
    {
    }

    file::file(const char *path)
    {
        m_access = s_file_access_provider->create_file_access(std::string(path));
    }

    file::file(const std::string& path)
    {
        m_access = s_file_access_provider->create_file_access(path);
    }

    file::file(const mge::path &path)
    {
        m_access = s_file_access_provider->create_file_access(path.generic_string());
    }

    file::file(const std::string& path, const std::string& name)
    {
        m_access = s_file_access_provider->create_file_access(path, name);
    }

    bool file::exists() const
    {
        return m_access->exists();
    }

    bool file::is_file() const
    {
        return m_access->is_file();
    }

    bool file::is_directory() const
    {
        return m_access->is_directory();
    }

    bool file::is_system_file() const
    {
        return m_access->is_system_file();
    }

    void file::mkdir()
    {
        m_access->mkdir();
    }

    void file::rmdir()
    {
        m_access->rmdir();
    }

    std::vector<file> file::list() const
    {
        std::vector<file> files;
        m_access->list(files);
        return files;
    }

    input_stream_ref file::open_for_input() const
    {
        return m_access->open_for_input();
    }

    const path& file::path() const
    {
        return m_access->path();
    }

    std::ostream&
    operator <<(std::ostream& os, const file& f)
    {
        return os << f.path();
    }
}

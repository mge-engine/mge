#include "mge/config.hpp"
#include "mge/core/file_access_factory.hpp"
#include "mge/core/file_access.hpp"
#include "mge/core/input_stream.hpp"
#include "mge/core/io_error.hpp"
#include "mge/core/file.hpp"
#include "mge/core/path.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/file_not_found.hpp"
#include "mge/core/filesystem_error.hpp"

#include <iostream>
#include <cstdio>
#include <cstring>

#ifndef MGE_OS_WINDOWS
#  include <sys/stat.h>
#  include <unistd.h>
#  include <dirent.h>
#else
#  include <windows.h>
#endif

namespace mge {
    class system_file_input_stream : public input_stream
    {
    public:
        system_file_input_stream(const std::string& filename)
        {
            m_file = fopen(filename.c_str(), "rb");
            if (!m_file) {
                if (errno == ENOENT) {
                    MGE_THROW(mge::file_not_found(),
                        "Cannot open file ", filename, " for reading");
                } else {
                    MGE_THROW(mge::io_error(),
                        "Cannot open file ", filename, " for reading");
                }
            }
        }

        ~system_file_input_stream()
        {
            if (m_file) {
                fclose(m_file);
            }
        }

        virtual offset_type position()
        {
            if (m_file) {
                return ftell(m_file);
            } else {
                return -1;
            }
        }

        virtual offset_type seek(offset_type offset, direction_type dir)
        {
#ifdef MGE_OS_WINDOWS
#  define fseek _fseeki64
#endif
            if (m_file) {
                int whence = this->whence(dir);
                if (fseek(m_file, offset, whence)) {
                    MGE_THROW(mge::io_error(),
                              "Seek operation failed");
                }
                return system_file_input_stream::position();
            } else {
                return -1;
            }
#ifdef MGE_OS_WINDOWS
#  undef fseek
#endif
        }
        input_stream::streamsize_type read()
        {
            return input_stream::read();
        }

        input_stream::streamsize_type on_read(void *destination,
                                              input_stream::streamsize_type size) override
        {
            if (size < 0) {
                MGE_THROW(mge::illegal_argument(),
                          "Read size must be >= 0");
            }

            streamsize_type readbytes = (streamsize_type) fread(
                            destination, 1, (size_t) size, m_file);
            if (readbytes != size) {
                if (!feof(m_file)) {
                    MGE_THROW(mge::io_error(),
                              "File read failed, failed to read",
                              size,
                              " bytes");
                }
            }

            return readbytes;
        }

    private:
        static int whence(direction_type dir)
        {
            switch (dir) {
            case POS_BEG:
                return SEEK_SET;
            case POS_CUR:
                return SEEK_CUR;
            case POS_END:
                return SEEK_END;
            default:
                MGE_THROW(mge::io_error(),
                          "Unsupported seek direction");
            }
        }
        FILE *m_file;
    };

    class system_file_access : public file_access
    {
    public:
        system_file_access(const std::string& path);
        system_file_access(const std::string& path,
                           const std::string& name);
        virtual ~system_file_access();

        virtual bool exists() const;
        virtual bool is_file() const;
        virtual bool is_directory() const;
        virtual bool is_system_file() const;

        virtual void mkdir();
        virtual void rmdir();
        virtual void list(std::vector<file>& files);

        virtual input_stream_ref open_for_input() const;
    private:
        path m_system_path;
    };

    system_file_access::system_file_access(const std::string& path)
                    : file_access(path), m_system_path(path)
    {
    }

    system_file_access::system_file_access(const std::string& path,
                                           const std::string& name)
                    : file_access(), m_system_path(path)
    {
        m_system_path /= name;
        set_path(m_system_path.generic_string());
    }

    system_file_access::~system_file_access()
    {
    }

    bool system_file_access::exists() const
    {
        std::string file_name = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        DWORD attrs = GetFileAttributes(file_name.c_str());
        SetLastError(NO_ERROR);
        return attrs != INVALID_FILE_ATTRIBUTES;
#else
        struct stat stat_info;
        int rc = ::stat(file_name.c_str(), &stat_info);
        errno = 0;
        return rc == 0;
#endif
    }

    bool system_file_access::is_file() const
    {
        std::string file_name = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        DWORD attrs = GetFileAttributes(file_name.c_str());
        if(attrs == INVALID_FILE_ATTRIBUTES) {
            return false;
        } else {
            return (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0;
        }
#else
        struct stat stat_info;
        int rc = ::stat(file_name.c_str(), &stat_info);
        return rc == 0 && ((stat_info.st_mode & S_IFDIR) == 0);
#endif

    }

    bool system_file_access::is_directory() const
    {
        std::string file_name = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        DWORD attrs = GetFileAttributes(file_name.c_str());
        if(attrs == INVALID_FILE_ATTRIBUTES) {
            return false;
        } else {
            return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
        }
#else
        struct stat stat_info;
        int rc = ::stat(file_name.c_str(), &stat_info);
        return rc == 0 && ((stat_info.st_mode & S_IFDIR) != 0);
#endif
    }

    bool system_file_access::is_system_file() const
    {
        return true;
    }

    void system_file_access::mkdir()
    {
        std::string file_name = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        BOOL rc = CreateDirectory(file_name.c_str(), NULL);
        if(!rc) {
            try {
                MGE_THROW_SYSTEM_ERROR;
            } catch(const exception& e) {
                MGE_THROW(mge::filesystem_error(),
                          "Cannot create directory  '",
                          file_path(),
                          "': ",
                          e.what()) << mge::exception::cause(e);
            }
        }
#else
        int rc = ::mkdir(file_name.c_str(),
                         S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
        if (rc == -1) {
            try {
                MGE_THROW_SYSTEM_ERROR;
            } catch(const exception& e) {
                MGE_THROW(mge::filesystem_error(),
                          "Cannot create directory  '",
                          file_path(),
                          "': ",
                          e.what()) << mge::exception::cause(e);
            }
        }
#endif
        return;
    }

    void system_file_access::rmdir()
    {
        if (!is_directory()) {
            MGE_THROW(mge::filesystem_error(),
                      "File '", file_path(), "' is not a directory");
        }
        std::string file_name = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        BOOL rc = RemoveDirectory(file_name.c_str());
        if(!rc) {
            try {
                MGE_THROW_SYSTEM_ERROR;
            } catch(const exception& e) {
                MGE_THROW(mge::filesystem_error(),
                          "Cannot remove directory  '",
                          file_path(),
                          "': ",
                          e.what()) << mge::exception::cause(e);
            }
        }
#else
        int rc = ::rmdir(file_name.c_str());
        if (rc != 0) {
            try {
                MGE_THROW_SYSTEMERROR;
            } catch(const exception& e) {
                throw MGE_EXCEPTION(filesystem_error)
                << "Cannot remove directory '"
                << get_path() << "': "
                << e.message();
            }
        }
#endif
    }

    input_stream_ref system_file_access::open_for_input() const
    {
        std::string s = m_system_path.string();
        return ::std::make_shared<system_file_input_stream>(s);
    }

    void system_file_access::list(std::vector<file>& files)
    {
        std::string pattern = m_system_path.string();
#ifdef MGE_OS_WINDOWS
        pattern += "\\*";
        WIN32_FIND_DATA find_info;
        HANDLE dir_handle = FindFirstFile(pattern.c_str(),
                        &find_info);
        if(dir_handle != INVALID_HANDLE_VALUE) {
            do {
                if(strcmp(find_info.cFileName, ".")!=0
                                && strcmp(find_info.cFileName, "..")!=0) {
                    std::string fname(find_info.cFileName);
                    path p(m_system_path);
                    p /= fname;
                    files.push_back(file(p.generic_string()));
                }
            }while(FindNextFile(dir_handle, &find_info));
            FindClose(dir_handle);
            SetLastError(NO_ERROR);
        }
#else
        DIR *dir = opendir(pattern.c_str());

        if (dir != NULL) {
            struct dirent *dp;
            while ((dp = readdir(dir)) != nullptr) {
                if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..")
                                != 0) {
                    path p(m_system_path, dp->d_name);
                    files.push_back(file(p.get_generic_string()));
                }
            }
            closedir(dir);
        }
#endif
    }

    class system_file_access_factory : public file_access_factory
    {
    public:
        system_file_access_factory() = default;
        virtual ~system_file_access_factory() = default;
        file_access_ref create_file_access(const std::string& path) override;
        file_access_ref create_file_access(const std::string& path,
                                           const std::string& filename) override;
    };

    MGE_REGISTER_IMPLEMENTATION(system_file_access_factory,
                                file_access_factory,
                                system, default, file);

    file_access_ref system_file_access_factory::create_file_access(
        const std::string& path)
    {
        auto result = std::make_shared<system_file_access>(path);
        return result;
    }

    file_access_ref system_file_access_factory::create_file_access(
        const std::string& path, const std::string& filename)
    {
        auto result = std::make_shared<system_file_access>(
            path, filename);
        return result;
    }
}

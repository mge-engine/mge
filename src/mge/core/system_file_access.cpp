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

#include "boost/filesystem.hpp"

#include <iostream>
#include <cstdio>
#include <cstring>


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
        boost::system::error_code ec;
        return boost::filesystem::exists(m_system_path, ec);
    }

    bool system_file_access::is_file() const
    {
        boost::system::error_code ec;
        return boost::filesystem::is_regular_file(m_system_path, ec);
    }

    bool system_file_access::is_directory() const
    {
        boost::system::error_code ec;
        return boost::filesystem::is_directory(m_system_path, ec);
    }

    bool system_file_access::is_system_file() const
    {
        return true;
    }

    void system_file_access::mkdir()
    {
        boost::system::error_code ec;
        if(!boost::filesystem::create_directory(m_system_path, ec)) {
            try {
                MGE_THROW_SYSTEM_ERROR(ec);
            } catch(const exception& e) {
                MGE_THROW(mge::filesystem_error(),
                          "Cannot create directory  '",
                          file_path(),
                          "': ",
                          e.what()) << mge::exception::cause(e);
            }
        }
    }

    void system_file_access::rmdir()
    {
        if (!is_directory()) {
            MGE_THROW(mge::filesystem_error(),
                      "File '", file_path(), "' is not a directory");
        }

        boost::system::error_code ec;
        if(!boost::filesystem::remove(m_system_path, ec)) {
            try {
                MGE_THROW_SYSTEM_ERROR(ec);
            } catch(const exception& e) {
                MGE_THROW(mge::filesystem_error(),
                          "Cannot remove directory  '",
                          file_path(),
                          "': ",
                          e.what()) << mge::exception::cause(e);
            }
        }

    }

    input_stream_ref system_file_access::open_for_input() const
    {
        std::string s = m_system_path.string();
        return ::std::make_shared<system_file_input_stream>(s);
    }

    void system_file_access::list(std::vector<file>& files)
    {
        for(boost::filesystem::directory_iterator it(m_system_path);
            it != boost::filesystem::directory_iterator();
            ++it) {
            files.push_back(file(it->path().generic_string()));
        }
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

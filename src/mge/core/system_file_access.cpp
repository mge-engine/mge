// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
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
    class system_file_output_stream : public output_stream
    {
    public:
        system_file_output_stream(const std::string& filename)
            : m_filename(filename)
            , m_file(nullptr)
        {
            m_file = fopen(filename.c_str(), "wb");
            if (!m_file) {
                if (errno == ENOENT) {
                    MGE_THROW(mge::file_not_found)
                        << "Cannot open file '" << filename << "' for writing";
                } else {
                    MGE_THROW(mge::io_error)
                        << "Cannot open file '" << filename << "' for writing";
                }
            }
        }

        ~system_file_output_stream()
        {
            if (m_file) {
                fclose(m_file);
            }
        }
    protected:
        void on_write(const void *buffer, streamsize_type size) override
        {
            size_t written = fwrite(buffer, 1, size, m_file);
            if (written != (size_t)size) {
                MGE_THROW(io_error) << "Cannot write to file '" << m_filename << "'";
            }
        }

        void on_flush() override
        {
            if (fflush(m_file)) {
                MGE_THROW(io_error)
                    << "Cannot flush file '" << m_filename << "'";
            }
        }
    private:
        std::string m_filename;
        FILE *m_file;
    };


    class system_file_input_stream : public input_stream
    {
    public:
        system_file_input_stream(const std::string& filename)
            : m_file(nullptr)
        {
            m_file = fopen(filename.c_str(), "rb");
            if (!m_file) {
                if (errno == ENOENT) {
                    MGE_THROW(mge::file_not_found)
                        << "Cannot open file '" << filename << "' for reading";
                } else {
                    MGE_THROW(mge::io_error)
                        << "Cannot open file '" << filename << "' for reading";
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
                    MGE_THROW(mge::io_error) << "Seek operation failed";
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
                MGE_THROW(mge::illegal_argument) <<  "Read size must be >= 0";
            }

            streamsize_type readbytes = (streamsize_type) fread(
                            destination, 1, (size_t) size, m_file);
            if (readbytes != size) {
                if (!feof(m_file)) {
                    MGE_THROW(mge::io_error)
                              << "File read failed, failed to read " << size << " bytes";
                } else {
                    if (readbytes == 0) {
                        readbytes = -1;
                    }
                }
            } else if (readbytes == 0) {
                if(feof(m_file)) {
                    return -1;
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
                MGE_THROW(mge::io_error) << "Unsupported seek direction: " << dir;
            }
        }
        FILE *m_file;
    };

    class system_file_access : public file_access
    {
    public:
        system_file_access(const mge::path& p)
            :file_access(p)
        {}

        ~system_file_access() = default;

        bool exists() const override;
        bool is_file() const override;
        bool is_directory() const override;
        bool is_system_file() const override;

        void mkdir() override;
        void rmdir() override;
        void list(std::vector<file>& files) override;

        input_stream_ref open_for_input() const override;
        output_stream_ref open_for_output() const override;
        size_t size() const override;
    };

    bool system_file_access::exists() const
    {
        boost::system::error_code ec;
        return boost::filesystem::exists(m_path, ec);
    }

    bool system_file_access::is_file() const
    {
        boost::system::error_code ec;
        return boost::filesystem::is_regular_file(m_path, ec);
    }

    bool system_file_access::is_directory() const
    {
        boost::system::error_code ec;
        return boost::filesystem::is_directory(m_path, ec);
    }

    bool system_file_access::is_system_file() const
    {
        return true;
    }

    void system_file_access::mkdir()
    {
        boost::system::error_code ec;
        if(!boost::filesystem::create_directory(m_path, ec)) {
            try {
                MGE_THROW_SYSTEM_ERROR(ec);
            } catch(const exception& e) {
                MGE_THROW_WITH_CAUSE(mge::filesystem_error, e)
                    << "Cannot create directory '"
                    << path()
                    << "': "
                    << e.what();
            }
        }
    }

    void system_file_access::rmdir()
    {
        if (!is_directory()) {
            MGE_THROW(mge::filesystem_error)
                      << "File '"
                      << path()
                      << "' is not a directory";
        }

        boost::system::error_code ec;
        if(!boost::filesystem::remove(m_path, ec)) {
            try {
                MGE_THROW_SYSTEM_ERROR(ec);
            } catch(const exception& e) {
                MGE_THROW_WITH_CAUSE(mge::filesystem_error, e)
                          << "Cannot remove directory  '"
                          << path()
                          << "': "
                          << e.what();
            }
        }

    }

    input_stream_ref system_file_access::open_for_input() const
    {
        std::string s = m_path.string();
        return ::std::make_shared<system_file_input_stream>(s);
    }

    output_stream_ref system_file_access::open_for_output() const
    {
        std::string s = m_path.string();
        return ::std::make_shared<system_file_output_stream>(s);
    }

    void system_file_access::list(std::vector<file>& files)
    {
        files.clear();
        if(exists() && is_directory()) {
            for(auto& p : boost::filesystem::directory_iterator(m_path)) {
                files.push_back(file(p.path()));
            }
        }
    }

    size_t system_file_access::size() const
    {
        boost::system::error_code ec;
        return boost::filesystem::file_size(m_path, ec);
    }

    class system_file_access_factory : public file_access_factory
    {
    public:
        system_file_access_factory() = default;
        virtual ~system_file_access_factory() = default;
        file_access_ref create_file_access(const mge::path& p) override;
    };

    MGE_REGISTER_IMPLEMENTATION(system_file_access_factory,
                                file_access_factory,
                                system, default, file);

    file_access_ref system_file_access_factory::create_file_access(
        const mge::path& p)
    {
        auto result = std::make_shared<system_file_access>(p);
        return result;
    }
}

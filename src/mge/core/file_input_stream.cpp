// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file_input_stream.hpp"
#include "mge/config.hpp"
#include "mge/core/io_exception.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include <cstdio>

namespace mge {
    file_input_stream::file_input_stream(const path& path)
        : m_path(path)
    {
#ifdef MGE_OS_WINDOWS
        m_file = ::_wfopen(path.native().c_str(), L"rb");
#else
        m_file = ::fopen(path.string().c_str(), "rb");
#endif
        if (!m_file) {
            MGE_CHECK_SYSTEM_ERROR(fopen);
        }
    }

    file_input_stream::~file_input_stream()
    {
        if (m_file) {
            ::fclose(m_file);
        }
    }

    input_stream::streamsize_type
    file_input_stream::on_read(void* buffer, input_stream::streamsize_type size)
    {
        try {
            input_stream::streamsize_type read =
                ::fread(buffer, 1, size, m_file);
            if (read != size) {
                if (!::feof(m_file)) {
                    MGE_CHECK_SYSTEM_ERROR(fread);
                }
            }
            return read;
        } catch (const mge::exception& e) {
            MGE_THROW_WITH_CAUSE(io_exception, e)
                << "Reading from file '" << m_path << "' failed";
        }
    }

    input_stream::offset_type file_input_stream::position()
    {
        try {
            auto result = ::ftell(m_file);
            if (result == -1) {
                MGE_CHECK_SYSTEM_ERROR(ftell);
            }
            return result;
        } catch (const mge::exception& ex) {
            MGE_THROW_WITH_CAUSE(io_exception, ex)
                << "Getting position in file '" << m_path << "' failed";
        }
    }

    input_stream::offset_type
    file_input_stream::seek(input_stream::offset_type    offset,
                            input_stream::direction_type dir)
    {
        try {
            int origin = 0;
            switch (dir) {
            case input_stream::POS_BEG:
                origin = SEEK_SET;
                break;
            case input_stream::POS_CUR:
                origin = SEEK_CUR;
                break;
            case input_stream::POS_END:
                origin = SEEK_END;
                break;
            default:
                MGE_THROW(illegal_argument)
                    << "Invalid direction type: " << dir;
            }
#ifdef MGE_OS_WINDOWS
            if (::_fseeki64(m_file, offset, origin) != 0) {
#else
            if (::fseeko64(m_file, offset, origin) != 0) {
#endif
                MGE_CHECK_SYSTEM_ERROR(fseek);
            }
        } catch (const mge::exception& ex) {
            MGE_THROW_WITH_CAUSE(io_exception, ex)
                << "Seeking in file '" << m_path << "' failed";
        }
        // not in try-catch, will raise own io_exception
        return position();
    }

} // namespace mge
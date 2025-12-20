// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file_output_stream.hpp"
#include "mge/config.hpp"
#include "mge/core/io_exception.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include <cstdio>

namespace mge {
    file_output_stream::file_output_stream(const path& path)
        : m_path(path)
    {
#ifdef MGE_OS_WINDOWS
        m_file = ::_wfopen(path.native().c_str(), L"wb");
#else
        m_file = ::fopen(path.string().c_str(), "wb");
#endif
        if (!m_file) {
            MGE_CHECK_SYSTEM_ERROR(fopen);
        }
    }

    file_output_stream::~file_output_stream()
    {
        if (m_file) {
            ::fclose(m_file);
        }
    }

    void file_output_stream::on_write(const void*                    buffer,
                                      output_stream::streamsize_type size)
    {
        try {
            output_stream::streamsize_type written =
                ::fwrite(buffer, 1, size, m_file);
            if (written != size) {
                MGE_CHECK_SYSTEM_ERROR(fwrite);
            }
        } catch (const mge::exception& e) {
            MGE_THROW_WITH_CAUSE(io_exception, e)
                << "Writing to file '" << m_path << "' failed";
        }
    }

    void file_output_stream::on_flush()
    {
        try {
            if (::fflush(m_file) != 0) {
                MGE_CHECK_SYSTEM_ERROR(fflush);
            }
        } catch (const mge::exception& e) {
            MGE_THROW_WITH_CAUSE(io_exception, e)
                << "Flushing file '" << m_path << "' failed";
        }
    }

} // namespace mge

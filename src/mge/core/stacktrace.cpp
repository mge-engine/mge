// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/stacktrace.hpp"
namespace mge {

    stacktrace::frame::frame(const void *address, std::string_view name,
                             std::string_view file, uint32_t line)
        : m_address(address), m_name(name), m_source_file(file),
          m_source_line(line)
    {}

    stacktrace::stacktrace() {}

    stacktrace::size_type stacktrace::size() const { return m_frames.size(); }

} // namespace mge

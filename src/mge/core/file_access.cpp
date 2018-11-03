// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file_access.hpp"
#include "mge/core/path.hpp"

namespace mge {
    file_access::file_access(const std::string& path)
                    : m_path(path)
    {}

    file_access::file_access()
    {}

    file_access::~file_access()
    {}

    const std::string&
    file_access::file_path() const
    {
        return m_path;
    }

    std::string file_access::file_name() const
    {
        path p(m_path);
        return p.file_name();
    }

    void file_access::set_path(const std::string& path)
    {
        m_path = path;
    }

    std::string file_access::file_extension() const
    {
        path p(m_path);
        return p.extension();
    }
}

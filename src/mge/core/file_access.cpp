// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/file_access.hpp"
#include "mge/core/path.hpp"

namespace mge {
    file_access::file_access(const mge::path& p)
        : m_path(p)
    {}


    const path&
    file_access::path() const
    {
        return m_path;
    }

}

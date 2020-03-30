// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/archive_access.hpp"
#include <iostream>
namespace mge {

    archive_access::archive_access(const file &f, open_mode m)
        : m_file(f), m_open_mode(m)
    {}

    void archive_access::gist(std::ostream &os) {}

} // namespace mge
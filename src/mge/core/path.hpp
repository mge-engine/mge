// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/dllexport.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 5204)
#endif
#include <boost/filesystem.hpp>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 5204)
#endif

#include <iosfwd>
#include <string>
#include <vector>

namespace mge {
    using boost::filesystem::canonical;
    using boost::filesystem::path;
    using boost::filesystem::weakly_canonical;
} // namespace mge

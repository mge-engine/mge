// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include "mge/core/gist.hpp"
#include <cstdint>
#include <vector>
namespace mge
{
    using buffer = std::vector<uint8_t>;

    GIST_OUTPUT(buffer)
    {
        return os << "buffer[" << g.value->size() << "]";
    }
}

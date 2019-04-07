// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
namespace mge {
    enum class MGE_GRAPHICS_EXPORT buffer_access
    {
        READ        = 1,
        WRITE       = 2,
        READ_WRITE  = 3
    };
}
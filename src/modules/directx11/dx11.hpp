// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#ifdef _MSC_VER
#    pragma warning(push, 0)
#endif

#include <d3d11.h>
#include <d3dcompiler.h>

#ifdef _MSC_VER
#    pragma warning(pop)
#endif

#include "mge/win32/com_unique_ptr.hpp"

#define RELEASE_NOTNULL(x)                                                     \
    if (x)                                                                     \
        x->Release();

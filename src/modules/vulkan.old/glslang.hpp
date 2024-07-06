// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4464)
#endif

#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 4464)
#endif

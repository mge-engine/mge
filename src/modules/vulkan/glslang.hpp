#pragma once
#include "mge/config.hpp"
#ifdef MGE_COMPILER_MSVC
#    pragma warning(disable : 4464)
#endif
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/ResourceLimits.h>
// #include <glslang/Public/resource_limits_c.h>
#ifdef MGE_COMPILER_MSVC
#    pragma warning(default : 4464)
#endif

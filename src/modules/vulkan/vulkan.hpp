// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>

# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
FIND_PACKAGE(Vulkan)
IF(Vulkan_FOUND)
    MESSAGE(STATUS "Vulkan found")
    FIND_PACKAGE(VulkanMemoryAllocator CONFIG REQUIRED)
ENDIF()



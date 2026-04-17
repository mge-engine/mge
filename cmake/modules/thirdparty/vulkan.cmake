# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL) 
IF(APPLE)
    FIND_PACKAGE(Vulkan COMPONENTS MoltenVK)
ELSE()
    FIND_PACKAGE(Vulkan)
ENDIF()
IF(Vulkan_FOUND)
    MESSAGE(STATUS "Vulkan found at ${Vulkan_INCLUDE_DIRS}")
    MESSAGE(STATUS "Vulkan library found at ${Vulkan_LIBRARY}")
    FIND_PACKAGE(VulkanMemoryAllocator CONFIG REQUIRED)
ENDIF()



# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
INCLUDE(FindVulkan)
IF(Vulkan_FOUND)
MESSAGE("-- Vulkan found")
ADD_LIBRARY(vulkan INTERFACE)
SET_PROPERTY(TARGET vulkan
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIRS}")
ENDIF()

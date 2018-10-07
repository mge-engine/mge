# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
INCLUDE(FindVulkan)
ADD_LIBRARY(vulkan INTERFACE)
SET_PROPERTY(TARGET vulkan
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIRS}")


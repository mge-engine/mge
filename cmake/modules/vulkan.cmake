# Copyright (c) 2017 by Alexander Schroeder
INCLUDE(FindVulkan)
ADD_LIBRARY(vulkan INTERFACE)
SET_PROPERTY(TARGET vulkan
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Vulkan_INCLUDE_DIRS}")


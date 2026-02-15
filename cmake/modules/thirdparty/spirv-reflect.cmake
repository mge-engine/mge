# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
INCLUDE(thirdparty/vulkan)
IF(Vulkan_FOUND)
    FIND_PACKAGE(unofficial-spirv-reflect CONFIG REQUIRED)
    IF(unofficial-spirv-reflect_FOUND)
        MESSAGE(STATUS "spirv-reflect found")
    ENDIF()
ENDIF()

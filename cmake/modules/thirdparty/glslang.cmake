# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
INCLUDE(thirdparty/vulkan)
IF(Vulkan_FOUND)
    FIND_PACKAGE(glslang CONFIG REQUIRED)
    #target_link_libraries(main PRIVATE glslang::glslang glslang::glslang-default-resource-limits glslang::SPIRV glslang::SPVRemapper)
    IF(glslang_FOUND)
        MESSAGE(STATUS "glslang found")
    ENDIF()
ENDIF()

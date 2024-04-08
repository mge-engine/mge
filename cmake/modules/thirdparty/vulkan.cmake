# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
FIND_PACKAGE(Vulkan COMPONENTS glslang SPIRV-Tools shaderc_combined REQUIRED)

#COMPONENTS glslang)
IF(Vulkan_FOUND)
    MESSAGE(STATUS "Vulkan found")
    GET_FILENAME_COMPONENT(VULKAN_LIB_DIR ${Vulkan_LIBRARIES} DIRECTORY)
    # Find debug library
    FIND_LIBRARY(VULKAN_GLSLANG_DEFAULT_RESOURCE_LIMITS_DEBUG
    NAMES glslang-default-resource-limitsd
    PATHS ${VULKAN_LIB_DIR}
    NO_DEFAULT_PATH)
    # Find release library
    FIND_LIBRARY(VULKAN_GLSLANG_DEFAULT_RESOURCE_LIMITS_RELEASE
    NAMES glslang-default-resource-limits
    PATHS ${VULKAN_LIB_DIR}
    NO_DEFAULT_PATH)

    ADD_LIBRARY("Vulkan::glslang-default-resource-limits" STATIC IMPORTED GLOBAL)
    SET_TARGET_PROPERTIES("Vulkan::glslang-default-resource-limits" PROPERTIES
                          IMPORTED_LOCATION_DEBUG ${VULKAN_GLSLANG_DEFAULT_RESOURCE_LIMITS_DEBUG}
                          IMPORTED_LOCATION_RELEASE ${VULKAN_GLSLANG_DEFAULT_RESOURCE_LIMITS_RELEASE})

ELSE()
    MESSAGE(FATAL_ERROR "Vulkan not found")
ENDIF()



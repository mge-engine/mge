# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_CUSTOM_COMMAND(OUTPUT vulkan_dump.h
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/extract_dump.py" "${Vulkan_INCLUDE_DIRS}/../share/vulkan/registry/vk.xml" "${CMAKE_CURRENT_BINARY_DIR}/vulkan_dump.h"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/extract_dump.py"
)

ADD_CUSTOM_COMMAND(OUTPUT vulkan_core.inc
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/extract_functions.py" "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan_core.h" "${CMAKE_CURRENT_BINARY_DIR}/vulkan_core.inc"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/extract_functions.py"
)

IF(WIN32)
    ADD_CUSTOM_COMMAND(OUTPUT vulkan_win32.inc
        COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/extract_functions.py" "${Vulkan_INCLUDE_DIRS}/vulkan/vulkan_win32.h" "${CMAKE_CURRENT_BINARY_DIR}/vulkan_win32.inc"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/extract_functions.py"
    )
ENDIF()

SET(mge_vulkan_module_SOURCES
    common.cpp
    render_system.cpp
    render_context.cpp
    swap_chain.cpp
    command_list.cpp
    shader.cpp
    vulkan_library.cpp
    window.cpp
    error.cpp)
IF(WIN32)
    SET(mge_vulkan_platform_SOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/vulkan_core.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/vulkan_win32.inc"
        "${CMAKE_CURRENT_BINARY_DIR}/vulkan_dump.h")
ENDIF()

ADD_LIBRARY(mge_module_vulkan MODULE
    ${mge_vulkan_module_SOURCES}
    ${mge_vulkan_platform_SOURCES}
)

TARGET_INCLUDE_DIRECTORIES(mge_module_vulkan PRIVATE "${CMAKE_BINARY_DIR}/src/modules/vulkan")

IF(WIN32)
    TARGET_LINK_LIBRARIES(mge_module_vulkan mge_win32 mgeapplication)
ENDIF()
TARGET_LINK_LIBRARIES(mge_module_vulkan mgecore mgegraphics vulkan glslang)
ADD_SUBDIRECTORY(test)





# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

IF(APPLE)
    SET(USE_GLFW_DEFAULT ON)
ELSE()
    SET(USE_GLFW_DEFAULT OFF)
ENDIF()
OPTION(USE_GLFW "Enable GLFW windowing support" ${USE_GLFW_DEFAULT})
IF(USE_GLFW)
    LIST(APPEND VCPKG_MANIFEST_FEATURES "glfw")
ENDIF()

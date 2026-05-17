# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

IF(WIN32)
    OPTION(USE_GLFW "Enable GLFW windowing support" OFF)
ELSE()
    # GLFW is the required UI platform on Linux/macOS.
    SET(USE_GLFW ON CACHE BOOL "Enable GLFW windowing support" FORCE)
ENDIF()

IF(USE_GLFW)
    LIST(APPEND VCPKG_MANIFEST_FEATURES "glfw")
ENDIF()

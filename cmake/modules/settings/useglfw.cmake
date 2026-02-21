# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
#
# This file is included early in CMakeLists.txt, before PROJECT(), because
# VCPKG_MANIFEST_FEATURES must be set prior to the vcpkg toolchain running
# during the project() call. Do not move this include after PROJECT().

IF(APPLE)
    SET(USE_GLFW_DEFAULT ON)
ELSE()
    SET(USE_GLFW_DEFAULT OFF)
ENDIF()
OPTION(USE_GLFW "Enable GLFW windowing support" ${USE_GLFW_DEFAULT})
IF(USE_GLFW)
    LIST(APPEND VCPKG_MANIFEST_FEATURES "glfw")
ENDIF()

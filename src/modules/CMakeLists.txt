# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

IF(OpenGL_FOUND)
    ADD_SUBDIRECTORY(opengl)
ELSE()
    MESSAGE("-- OpenGL not found, not building opengl render system")
ENDIF()
IF(Vulkan_FOUND)
ADD_SUBDIRECTORY(vulkan)
ELSE()
    MESSAGE("-- Vulkan not found, not building vulkan render system")
ENDIF()


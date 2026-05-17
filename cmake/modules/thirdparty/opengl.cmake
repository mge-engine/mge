# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
IF(POLICY CMP0072)
    CMAKE_POLICY(SET CMP0072 NEW)
ENDIF()
SET(OpenGL_GL_PREFERENCE GLVND)
FIND_PACKAGE(OpenGL)
IF(OpenGL_FOUND)
    MESSAGE("-- OpenGL found")
ENDIF()

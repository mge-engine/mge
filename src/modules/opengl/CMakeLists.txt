# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mge_opengl_module_SOURCES
    common.cpp
    window.cpp
    render_system.cpp)
ADD_LIBRARY(mge_module_opengl MODULE
            ${mge_opengl_module_SOURCES})
TARGET_INCLUDE_DIRECTORIES(mge_module_opengl PRIVATE ${OPENGL_INCLUDE_DIRS})
IF(WIN32)
    TARGET_LINK_LIBRARIES(mge_module_opengl mge_win32 mgeapplication)
ENDIF()
TARGET_LINK_LIBRARIES(mge_module_opengl mgecore mgegraphics gl3w ${OPENGL_LIBRARIES})


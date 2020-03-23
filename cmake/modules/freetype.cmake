# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
FIND_PATH(FREETYPE_INCLUDE_DIRS ft2build.h)
FIND_LIBRARY(FREETYPE_LINK_LIBRARIES freetype)
ADD_LIBRARY(freetype UNKNOWN IMPORTED)
SET_PROPERTY(TARGET freetype
             PROPERTY IMPORTED_LOCATION "${FREETYPE_LINK_LIBRARIES}")
SET_PROPERTY(TARGET freetype
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${FREETYPE_INCLUDE_DIRS}")
#INCLUDE(functions)
#IF(WIN32)
#    IF(VCPKG_TOOLCHAIN STREQUAL "ON")
##_VCPKG_INSTALLED_DIR
#        MESSAGE("-- Setup freetype libraries from vcpkg")
#        COPY_FILE("${_VCPKG_INSTALLED_DIR}/x64-windows/bin/freetype.dll"
#            "${CMAKE_BINARY_DIR}/freetype.dll")
#        COPY_FILE("${_VCPKG_INSTALLED_DIR}/x64-windows/bin/freetype.pdb"
#            "${CMAKE_BINARY_DIR}/freetype.pdb")
#    ELSE()
#        MESSAGE(FATAL_ERROR "Need vcpkg info for freetype")
#    ENDIF()
#ELSE()
#    MESSAGE(FATAL_ERROR "Unsupported Setup")
#ENDIF()

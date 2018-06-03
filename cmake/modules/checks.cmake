# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

INCLUDE(CheckIncludeFiles)

IF(CMAKE_CXX_COMPILER MATCHES ".*clang")
    SET(CMAKE_COMPILER_IS_CLANGXX 1)
ENDIF()

INCLUDE(CheckCXX11Features)

CHECK_CXX11_FEATURES()

IF(NOT "${HAVE_CXX11}")
    MESSAGE(FATAL_ERROR "MGE requires basic C++ 11 support")
ENDIF()



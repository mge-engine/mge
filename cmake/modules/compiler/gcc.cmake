# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    MESSAGE(STATUS "-- Configuring for GCC")
    ADD_DEFINITIONS("-Wall")
    ADD_DEFINITIONS("-std=c++23")
    ADD_DEFINITIONS("-Wno-unused-but-set-variable")
ENDIF()
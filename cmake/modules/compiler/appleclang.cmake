# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    MESSAGE(STATUS "-- Configuring for Apple Clang")
    ADD_DEFINITIONS("-Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
ENDIF()

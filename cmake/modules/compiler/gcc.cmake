# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    ADD_DEFINITIONS("-std=gnu++20")
    #ADD_DEFINITIONS("-Wall")
    #ADD_DEFINITIONS("-Wextra")
    #ADD_DEFINITIONS("-Wpedantic")
    #ADD_DEFINITIONS("-Werror")
ENDIF()
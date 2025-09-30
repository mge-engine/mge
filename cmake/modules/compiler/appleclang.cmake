# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

if(APPLE)
    message(STATUS "Using Apple Clang compiler")
    set(CMAKE_CXX_STANDARD 20)          # Use C++20
    set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure the standard is required
    ADD_DEFINITIONS("-Wall")
endif() 
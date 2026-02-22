# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

INCLUDE_GUARD(GLOBAL)

FIND_PROGRAM(SCOOP_EXECUTABLE scoop)

if(SCOOP_EXECUTABLE)
    MESSAGE(STATUS "Scoop found at ${SCOOP_EXECUTABLE}")
endif()

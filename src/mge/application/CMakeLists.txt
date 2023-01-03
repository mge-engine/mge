# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeapplication_headers
    dllexport.hpp
    application.hpp
    loop_target.hpp
    loop.hpp
)

SET(mgeapplication_sources
    application.cpp
    loop.cpp
    simple_loop.cpp
    fixed_step_loop.cpp
)

MGE_LIBRARY(
    TARGET  mgeapplication
    TYPE    SHARED
    SOURCES
    ${mgeapplication_sources}
    ${mgeapplication_headers}
)
TARGET_LINK_LIBRARIES(mgeapplication mgecore boost-all)

ADD_SUBDIRECTORY(test)


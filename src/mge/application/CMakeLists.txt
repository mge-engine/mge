# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgeapplication
    TYPE    SHARED
    SOURCES
        application.cpp
        loop.cpp
        simple_loop.cpp

        ${CMAKE_SOURCE_DIR}/include/mge/application/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/application/application.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/application/loop_target.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/application/loop.hpp
)
TARGET_LINK_LIBRARIES(mgeapplication mgecore boost-all)

ADD_SUBDIRECTORY(test)


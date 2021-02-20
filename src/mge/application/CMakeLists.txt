# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgeapplication
    TYPE    SHARED
    SOURCES
        application.cpp

        ${CMAKE_SOURCE_DIR}/include/mge/application/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/application/application.hpp
)
TARGET_LINK_LIBRARIES(mgeapplication mgecore boost-all)

ADD_SUBDIRECTORY(test)


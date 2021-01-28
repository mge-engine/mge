# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgecore
    TYPE    SHARED
    SOURCES
        executable_name.cpp
        type_name.cpp
        log.cpp)
TARGET_LINK_LIBRARIES(mgecore boost-all)
ADD_SUBDIRECTORY(test)


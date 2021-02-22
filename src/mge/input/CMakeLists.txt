# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgeinput
    TYPE    SHARED
    SOURCES
        key.cpp
        modifier.cpp
        keyboard_state.cpp
        mouse_state.cpp
        input_state.cpp

        ${CMAKE_SOURCE_DIR}/include/mge/input/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/input/key.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/input/modifier.hpp

)
TARGET_LINK_LIBRARIES(mgeinput boost-all)
ADD_SUBDIRECTORY(test)

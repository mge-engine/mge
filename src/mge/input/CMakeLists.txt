# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(mgeinput_sources
   key.cpp
   key_action.cpp
   modifier.cpp
   keyboard_state.cpp
   mouse_state.cpp
   mouse_action.cpp
   input_state.cpp
   input_handler.cpp
)

SET(mgeinput_headers
    dllexport.hpp
    key.hpp
    key_action.hpp
    modifier.hpp
    input_state.hpp
    input_handler.hpp
    keyboard_state.hpp
    mouse_state.hpp
    mouse_action.hpp
)


MGE_LIBRARY(
    TARGET  mgeinput
    TYPE    SHARED
    SOURCES
    ${mgeinput_sources}
    ${mgeinput_headers}
)
TARGET_LINK_LIBRARIES(mgeinput boost-all)
ADD_SUBDIRECTORY(test)

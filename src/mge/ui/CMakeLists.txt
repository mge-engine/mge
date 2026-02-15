# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeui_sources
   common.cpp
   immediate.cpp
)

SET(mgeui_headers
    dllexport.hpp
    immediate.hpp
)


MGE_LIBRARY(
    TARGET  mgeui
    TYPE    SHARED
    SOURCES
    ${mgeui_sources}
    ${mgeui_headers}
)
TARGET_LINK_LIBRARIES(mgeui mgecore mgeinput mgegraphics nuklear boost-all)
ADD_SUBDIRECTORY(test)

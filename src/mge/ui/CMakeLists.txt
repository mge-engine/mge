# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeui_sources
    dummy.cpp
)

SET(mgeui_headers
)

MGE_LIBRARY(
    TARGET mgeui
    TYPE SHARED
    SOURCES
    ${mgeui_sources}
    ${mgeui_headers}
)
TARGET_LINK_LIBRARIES(mgeui mgecore mgegraphics boost-all)
#ADD_SUBDIRECTORY(test)

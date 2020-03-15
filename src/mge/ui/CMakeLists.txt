# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgeui_sources
    context.cpp
    font.cpp
    window.cpp
    menu_bar.cpp
    menu.cpp
    context_object.cpp
    font.cpp
)

ADD_LIBRARY(mgeui
            SHARED
            ${mgeui_sources})

TARGET_ADD_DEFINE(mgeui BUILD_MGE_UI)
TARGET_LINK_LIBRARIES(mgeui mgegraphics mgeinput mgecore)

ADD_SUBDIRECTORY(test)


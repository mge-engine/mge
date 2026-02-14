# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgeui_sources
   ui.cpp
)

SET(mgeui_headers
    dllexport.hpp
    ui_context.hpp
)


MGE_LIBRARY(
    TARGET  mgeui
    TYPE    SHARED
    SOURCES
    ${mgeui_sources}
    ${mgeui_headers}
)
TARGET_LINK_LIBRARIES(mgeui mgecore nuklear boost-all)
ADD_SUBDIRECTORY(test)

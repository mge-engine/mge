# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_win32 STATIC
    monitor.cpp
    window.cpp
    win32.cpp)

TARGET_LINK_LIBRARIES(mge_win32 mgecore)
ADD_SUBDIRECTORY(test)

# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_win32 STATIC
    monitor.cpp
    window.cpp)

TARGET_LINK_LIBRARIES(mge_win32 mgecore)
ADD_SUBDIRECTORY(test)

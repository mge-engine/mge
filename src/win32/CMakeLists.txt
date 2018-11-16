# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mge_win32 STATIC
    monitor.cpp
    win32.cpp)
TARGET_LINK_LIBRARIES(mge_win32 boost-all)
ADD_SUBDIRECTORY(test)

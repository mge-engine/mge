# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgeapplication_sources
    update_thread.cpp
    application.cpp)

ADD_LIBRARY(mgeapplication
            SHARED
            ${mgeapplication_sources})

TARGET_ADD_DEFINE(mgeapplication BUILD_MGE_APPLICATION)
TARGET_LINK_LIBRARIES(mgeapplication mgecore)

ADD_SUBDIRECTORY(test)

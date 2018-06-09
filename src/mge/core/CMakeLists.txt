# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgecore_sources
    executable_name.cpp
)

ADD_LIBRARY(mgecore
            SHARED
            ${mgecore_sources})

TARGET_ADD_DEFINE(mgecore BUILD_MGE_CORE)
#TARGET_LINK_LIBRARIES(mgecore boost)

IF(WIN32)
    TARGET_LINK_LIBRARIES(mgecore dbghelp)
ENDIF()

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES(mgecore pthread dl)
ENDIF()

#ADD_SUBDIRECTORY(test)


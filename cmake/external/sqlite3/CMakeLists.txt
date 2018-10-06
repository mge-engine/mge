# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

CMAKE_MINIMUM_REQUIRED(VERSION "3.2")

# will use this with a dynamic runtime, so we must use -MD accordingly
IF(MSVC)
    IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
        ADD_DEFINITIONS("-MDd")
    ENDIF()
    IF(CMAKE_BUILD_TYPE STREQUAL "Release")
        ADD_DEFINITIONS("-MD")
    ENDIF()
    IF(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        ADD_DEFINITIONS("-MD")
    ENDIF()
ENDIF()

ADD_LIBRARY(sqlite3 sqlite3.c)
ADD_EXECUTABLE(mgesqlite sqlite3.c shell.c)
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_COMPILE_OPTIONS(sqlite3 PRIVATE "-fPIC")
    TARGET_LINK_LIBRARIES(mgesqlite pthread dl)
ENDIF()



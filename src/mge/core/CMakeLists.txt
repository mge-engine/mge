# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgecore_sources
    executable_name.cpp
    string_pool.cpp
    stacktrace.cpp
    exception.cpp
    system_error.cpp
    stdexceptions.cpp
    component.cpp
    type_name.cpp
    thread.cpp
    crash.cpp
    log_severity.cpp
    log_record.cpp
    log_sink.cpp
    log_formatter.cpp
    text_log_formatter.cpp
    stdout_log_sink.cpp
    log.cpp
    memory.cpp
    new.cpp
    configuration.cpp
    configuration_database.cpp
    configurable.cpp
    path.cpp
    async_executor.cpp
)

ADD_LIBRARY(mgecore
            SHARED
            ${mgecore_sources})

TARGET_ADD_DEFINE(mgecore BUILD_MGE_CORE)
TARGET_LINK_LIBRARIES(mgecore boost sqlite3)

IF(WIN32)
    TARGET_LINK_LIBRARIES(mgecore dbghelp)
ENDIF()

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES(mgecore pthread dl)
ENDIF()

ADD_SUBDIRECTORY(test)


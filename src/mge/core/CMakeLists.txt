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
    thread_group.cpp
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
    async_executor.cpp
    task.cpp
    shared_library.cpp
    clock.cpp
    module.cpp
    file_not_found.cpp
    filesystem_error.cpp
    input_stream.cpp
    file_access.cpp
    file_access_factory.cpp
    file.cpp
    io_error.cpp
    system_file_access.cpp
    version.cpp
    properties.cpp
    property_object.cpp
    output_stream.cpp
    archive.cpp
    archive_access.cpp
    archive_access_factory.cpp
    open_mode.cpp

    ${CMAKE_BINARY_DIR}/src/mge/version.hpp
    ${CMAKE_BINARY_DIR}/src/mge/config.hpp

    array_size.hpp
    version.hpp
)

ADD_LIBRARY(mgecore
            SHARED
            ${mgecore_sources})

TARGET_ADD_DEFINE(mgecore BUILD_MGE_CORE)
TARGET_LINK_LIBRARIES(mgecore boost-all sqlite3)

IF(WIN32)
    TARGET_LINK_LIBRARIES(mgecore dbghelp)
ENDIF()

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    TARGET_LINK_LIBRARIES(mgecore pthread dl)
ENDIF()
DOCUMENT_TARGET(mgecore)
ADD_SUBDIRECTORY(test)


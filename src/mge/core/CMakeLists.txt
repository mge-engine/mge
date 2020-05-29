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
    file_log_sink.cpp
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
    debug_break.cpp
    memory_resource.cpp
    artifact.cpp
    license.cpp

    ${CMAKE_BINARY_DIR}/src/mge/version.hpp
    ${CMAKE_BINARY_DIR}/src/mge/config.hpp

    archive_access_factory.hpp
    archive_access.hpp
    archive.hpp
    array_size.hpp
    artifact.hpp
    async_executor.hpp
    bit.hpp
    bool_value.hpp
    buffer.hpp
    checked_cast.hpp
    clear_function.hpp
    clock.hpp
    component.hpp
    configurable.hpp
    configuration.hpp
    contains.hpp
    copy_struct.hpp
    crash.hpp
    debug_break.hpp
    enum_set.hpp
    exception.hpp
    executable_name.hpp
    file_access_factory.hpp
    file_access.hpp
    file_not_found.hpp
    file.hpp
    filesystem_error.hpp
    format_string.hpp
    function_map.hpp
    gist.hpp
    input_stream.hpp
    io_error.hpp
    lexical_cast.hpp
    license.hpp
    log_formatter.hpp
    log_function_guard.hpp
    log_record.hpp
    log_severity.hpp
    log_sink.hpp
    log.hpp
    memory_resource.hpp
    memory.hpp
    module.hpp
    new.hpp
    open_mode.hpp
    output_stream.hpp
    path.hpp
    properties.hpp
    property_object.hpp
    sequence.hpp
    shared_library.hpp
    singleton.hpp
    small_vector.hpp
    stacktrace.hpp
    stdexceptions.hpp
    stream_values.hpp
    streamutils.hpp
    string_pool.hpp
    system_error.hpp
    tagged.hpp
    task_queue.hpp
    task.hpp
    thread_group.hpp
    to_utf8.hpp
    to_void_ptr.hpp
    type_name.hpp
    type_traits.hpp
    types.hpp
    unlikely.hpp
    version.hpp
    void_function.hpp
    zero_memory.hpp
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


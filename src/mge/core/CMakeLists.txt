# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(mgecore_sources
    platform.cpp
    executable_name.cpp
    type_name.cpp
    configuration.cpp
    parameter.cpp
    trace_level.cpp
    trace_topic.cpp
    trace_sink.cpp
    memory_trace_sink.cpp
    stream_trace_sink.cpp
    trace_formatter.cpp
    simple_trace_formatter.cpp
    trace.cpp
    statistics.cpp
    string_pool.cpp
    stacktrace.cpp
    exception.cpp
    stdexceptions.cpp
    component.cpp
    shared_library.cpp
    system_error.cpp
    module.cpp
    crash.cpp
    thread.cpp
    thread_group.cpp
    semantic_version.cpp
    output_stream.cpp
    input_stream.cpp
    properties.cpp
)

SET(mgecore_headers
    ${CMAKE_SOURCE_DIR}/src/mge/config.hpp
    ${CMAKE_SOURCE_DIR}/src/mge/dllexport.hpp
    executable_name.hpp
    make_string_view.hpp
    memory.hpp
    overloaded.hpp
    configuration.hpp
    dllexport.hpp
    parameter.hpp
    type_name.hpp
    memory_trace_sink.hpp
    simple_trace_formatter.hpp
    singleton.hpp
    statistics.hpp
    stream_trace_sink.hpp
    trace_formatter.hpp
    trace_level.hpp
    trace_record.hpp
    trace_sink.hpp
    trace_topic.hpp
    trace.hpp
    string_pool.hpp
    stacktrace.hpp
    exception.hpp
    stdexceptions.hpp
    component.hpp
    contains.hpp
    noncopyable.hpp
    callback_map.hpp
    shared_library.hpp
    system_error.hpp
    module.hpp
    crash.hpp
    copy_struct.hpp
    zero_memory.hpp
    thread.hpp
    thread_group.hpp
    semantic_version.hpp
    output_stream.hpp
    input_stream.hpp
    properties.hpp
)

MGE_LIBRARY(
    TARGET  mgecore
    TYPE    SHARED
    SOURCES
    ${mgecore_sources}
    ${mgecore_headers}
)

TARGET_LINK_LIBRARIES(mgecore boost-all dbghelp)
ADD_SUBDIRECTORY(test)


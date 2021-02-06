# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgecore
    TYPE    SHARED
    SOURCES
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
        statistics.cpp)
TARGET_LINK_LIBRARIES(mgecore boost-all)
ADD_SUBDIRECTORY(test)


# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgecore
    TYPE    SHARED
    SOURCES
        executable_name.cpp
        type_name.cpp
        trace_level.cpp
        trace_topic.cpp
        trace_sink.cpp
        memory_trace_sink.cpp
        trace_formatter.cpp
        simple_trace_formatter.cpp
        trace.cpp)
TARGET_LINK_LIBRARIES(mgecore boost-all)
ADD_SUBDIRECTORY(test)


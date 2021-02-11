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
        statistics.cpp

        ${CMAKE_SOURCE_DIR}/include/mge/config.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/executable_name.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/make_string_view.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/memory.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/overloaded.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/configuration.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/parameter.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/type_name.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/memory_trace_sink.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/simple_trace_formatter.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/singleton.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/statistics.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/stream_trace_sink.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace_formatter.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace_level.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace_record.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace_sink.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace_topic.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/core/trace.hpp
    )

TARGET_LINK_LIBRARIES(mgecore boost-all)
ADD_SUBDIRECTORY(test)


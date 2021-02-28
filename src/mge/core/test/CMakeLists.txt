# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGECORE_TESTS
    executable_name
    type_name
    trace
    trace_level
    trace_topic
    memory_trace_sink
    stream_trace_sink
    simple_trace_formatter
    parameter
    configuration
    statistics
    stacktrace
    string_pool
    exception
    component
    enum_set
    callback_map)

FOREACH(TEST_NAME ${MGECORE_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgecore
    )
ENDFOREACH()


# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGECORE_TEST_SOURCES
    test_executable_name.cpp
    test_type_name.cpp
    test_trace.cpp
    test_trace_level.cpp
    test_trace_topic.cpp
    test_memory_trace_sink.cpp
    test_stream_trace_sink.cpp
    test_parameter.cpp
    test_simple_trace_formatter.cpp
    test_configuration.cpp
    test_statistics.cpp
    test_stacktrace.cpp
    test_string_pool.cpp
    test_exception.cpp
    test_component.cpp
    test_enum_set.cpp
    test_callback_map.cpp
    test_crash.cpp
    test_copy_struct.cpp
    test_semantic_version.cpp
    test_small_vector.cpp
    test_thread.cpp
    test_thread_group.cpp
    test_output_stream.cpp
    test_checked_cast.cpp
    test_callable.cpp
    test_gist.cpp
    test_call_debugger.cpp
    test_nargs.cpp
    test_sequenced.cpp
    test_lru_cache.cpp
    test_lexical_cast.cpp
    test_concepts.cpp
    test_json.cpp
    test_enum.cpp
)

MGE_TEST(
    TARGET      test_core
    SOURCES     ${MGECORE_TEST_SOURCES}
    LIBRARIES   mgecore
)



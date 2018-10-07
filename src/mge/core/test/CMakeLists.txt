# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(coretest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_executable_name.cpp
  test_exception.cpp
  test_string_pool.cpp
  test_stacktrace.cpp
  test_stream_values.cpp
  test_type_name.cpp
  test_component.cpp
  test_thread.cpp
  test_log_severity.cpp
  test_log_sink.cpp
  test_log.cpp
  test_path.cpp
  test_configuration.cpp
  test_text_log_formatter.cpp
  test_bool_value.cpp
)
TARGET_LINK_LIBRARIES(coretest mgecore gmock)
TARGET_IS_TEST(coretest)

ADD_EXECUTABLE(coretest_crash
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_crash.cpp
)
TARGET_LINK_LIBRARIES(coretest_crash mgecore gmock)
TARGET_IS_TEST(coretest_crash)


ADD_EXECUTABLE(corebenchmark
  ${CMAKE_SOURCE_DIR}/src/bench/benchmark_main.cpp
  bench_format_string.cpp
  bench_exception.cpp
  bench_lexical_cast.cpp
  bench_stacktrace.cpp
  bench_memory.cpp
)
TARGET_LINK_LIBRARIES(corebenchmark mgecore googlebenchmark)

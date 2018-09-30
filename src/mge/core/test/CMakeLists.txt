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
)
TARGET_LINK_LIBRARIES(coretest mgecore gmock)
TARGET_IS_TEST(coretest)

ADD_EXECUTABLE(corebenchmark
  ${CMAKE_SOURCE_DIR}/src/bench/benchmark_main.cpp
  bench_format_string.cpp
  bench_exception.cpp
  bench_lexical_cast.cpp
  bench_stacktrace.cpp
)
TARGET_LINK_LIBRARIES(corebenchmark mgecore googlebenchmark)

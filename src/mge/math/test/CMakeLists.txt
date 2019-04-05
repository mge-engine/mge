# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(mathtest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_vec2.cpp
    test_vec3.cpp
    test_aabox.cpp)
TARGET_LINK_LIBRARIES(mathtest mgemath mgecore gmock)
TARGET_IS_TEST(mathtest)

ADD_EXECUTABLE(mathbenchmark
  bench_vec3.cpp
  bench_mat4.cpp
  ${CMAKE_SOURCE_DIR}/src/bench/benchmark_main.cpp
)
TARGET_LINK_LIBRARIES(mathbenchmark googlebenchmark mgemath mgecore)

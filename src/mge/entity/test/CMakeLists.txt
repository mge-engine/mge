# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(entitytest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_entity_registry.cpp
    test_component.cpp)
TARGET_LINK_LIBRARIES(entitytest mgeentity mgecore  mgemath gmock)
TARGET_IS_TEST(entitytest)
    
ADD_EXECUTABLE(entitybenchmark
    bench_entity_registry.cpp
    bench_component.cpp
    ${CMAKE_SOURCE_DIR}/src/bench/benchmark_main.cpp)
TARGET_LINK_LIBRARIES(entitybenchmark googlebenchmark mgeentity mgecore)

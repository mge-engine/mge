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


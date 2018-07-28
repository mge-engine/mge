# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(mathtest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_vec2.cpp)
TARGET_LINK_LIBRARIES(mathtest mgecore gmock)
TARGET_IS_TEST(mathtest)


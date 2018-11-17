# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(reflectiontest
    test_module.cpp
    test_type_enum.cpp
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp)

TARGET_LINK_LIBRARIES(reflectiontest mgecore mgereflection gmock)
TARGET_IS_TEST(reflectiontest)

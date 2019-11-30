# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(ziptest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    zip_test.cpp)
TARGET_LINK_LIBRARIES(ziptest mgecore gmock)
TARGET_IS_TEST(ziptest)

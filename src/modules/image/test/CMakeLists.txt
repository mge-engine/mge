# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(imagetest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_image.cpp)
TARGET_LINK_LIBRARIES(imagetest mgeasset mgecore gmock)
TARGET_IS_TEST(imagetest)

ADD_EXECUTABLE(jpegtest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_jpeg.cpp)
TARGET_LINK_LIBRARIES(jpegtest mgeasset mgecore gmock)
TARGET_IS_TEST(jpegtest)

# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(coretest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_executable_name.cpp
  test_exception.cpp
)
TARGET_LINK_LIBRARIES(coretest mgecore gmock)
TARGET_IS_TEST(coretest)


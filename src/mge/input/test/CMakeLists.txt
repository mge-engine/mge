# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(inputtest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_key.cpp
  test_key_action.cpp
  test_modifier.cpp
)
TARGET_LINK_LIBRARIES(inputtest mgeinput mgecore gmock)
TARGET_IS_TEST(inputtest)

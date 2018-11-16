# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(win32test
  ${CMAKE_SOURCE_DIR}/src/test/test_simple_main.cpp
  test_monitor.cpp
)
TARGET_LINK_LIBRARIES(win32test mge_win32 mgecore mgegraphics gmock)
# interactive, will fail if no monitor present
#TARGET_IS_TEST(win32test)


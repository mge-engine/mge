# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(applicationtest
  test_application.cpp)
TARGET_LINK_LIBRARIES(applicationtest mgeapplication mgecore gmock)
TARGET_IS_TEST(applicationtest)

ADD_EXECUTABLE(applicationtest_mainloop
  test_application_mainloop.cpp)
TARGET_LINK_LIBRARIES(applicationtest_mainloop mgeapplication mgecore gmock)
TARGET_IS_TEST(applicationtest_mainloop)

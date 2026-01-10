# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(WIN32_TEST_SOURCES
    test_win32_monitor.cpp
    test_win32_window.cpp)

MGE_TEST(
    TARGET      test_win32
    SOURCES     ${WIN32_TEST_SOURCES}
    NEEDSDISPLAY
    DISABLED
    LIBRARIES   mge_win32 mgecore mgegraphics mgeapplication mgeinput
)
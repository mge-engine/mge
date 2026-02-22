# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.

SET(GLFW_TEST_SOURCES
    test_glfw_monitor.cpp
    test_glfw_window.cpp)

MGE_TEST(
    TARGET      test_glfw
    SOURCES     ${GLFW_TEST_SOURCES}
    NEEDSDISPLAY
    LIBRARIES   mge_glfw mgecore mgegraphics mgeapplication mgeinput
)

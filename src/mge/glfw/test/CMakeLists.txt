# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(GLFW_TEST_SOURCES
    test_glfw_monitor.cpp)

MGE_TEST(
    TARGET      test_glfw
    SOURCES     ${GLFW_TEST_SOURCES}
    NEEDSDISPLAY
    LIBRARIES   mge_glfw mgecore mgegraphics mgeapplication mgeinput
)

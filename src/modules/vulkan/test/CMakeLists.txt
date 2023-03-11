# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(VULKAN_TEST_SOURCES
    vulkan_test.cpp
    test_render_system.cpp
    test_command_list.cpp)

MGE_TEST(
    TARGET      test_vulkan
    SOURCES     ${VULKAN_TEST_SOURCES}
    NOMAIN NEEDSDISPLAY
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput
)
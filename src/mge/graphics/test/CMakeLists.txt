# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEGRAPHICS_TEST_SOURCES
    test_extent.cpp
    test_point.cpp
    test_window.cpp
    test_video_mode.cpp
    test_rgb_color.cpp
    test_rgba_color.cpp
    test_data_type.cpp
    test_hardware_buffer.cpp
)

MGE_TEST(
    TARGET      test_graphics
    SOURCES     ${MGEGRAPHICS_TEST_SOURCES}
    LIBRARIES   mgegraphics mgecore
)


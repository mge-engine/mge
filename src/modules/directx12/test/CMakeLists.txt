# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(DX12_TEST_SOURCES
    dx12_test.cpp
    test_render_system.cpp
    # test_index_buffer.cpp
    )

MGE_TEST(
    TARGET      test_dx12
    SOURCES     ${DX12_TEST_SOURCES}
    NOMAIN NEEDSDISPLAY
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput
)
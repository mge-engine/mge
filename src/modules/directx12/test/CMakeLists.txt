# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(DX12_TEST_SOURCES
    dx12_test.cpp
    test_render_system.cpp
    test_index_buffer.cpp
    test_vertex_buffer.cpp
    )

MGE_TEST(
    TARGET      test_dx12
    SOURCES     ${DX12_TEST_SOURCES}
    NOMAIN 
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput
)
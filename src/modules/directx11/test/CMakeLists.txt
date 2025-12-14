# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(DX11_TEST_SOURCES
    dx11_test.cpp
    test_render_system.cpp
    test_render_context.cpp
    test_index_buffer.cpp
    test_vertex_buffer.cpp
    test_shader.cpp
    test_program.cpp
    test_command_list.cpp)

MGE_TEST(
    TARGET      test_dx11
    SOURCES     ${DX11_TEST_SOURCES}
    NOMAIN NEEDSDISPLAY
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput benchmark
)
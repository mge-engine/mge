# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(OPENGL_TEST_SOURCES
    opengl_test.cpp
    test_render_system.cpp
    test_index_buffer.cpp
    test_shader.cpp
    test_program.cpp
    test_texture.cpp)

MGE_TEST(
    TARGET      test_opengl
    SOURCES     ${OPENGL_TEST_SOURCES}
    NOMAIN NEEDSDISPLAY
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput benchmark
)
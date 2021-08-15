# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(OPENGL_TEST_SOURCES
    opengl_test.cpp
    test_render_system.cpp
    test_index_buffer.cpp)

MGE_TEST(
    TARGET      test_opengl
    SOURCES     ${OPENGL_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgecore mgegraphics mgeapplication mgeinput
)
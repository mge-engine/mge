# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(opengltest
    test_main.cpp
    test_render_system.cpp
    test_window.cpp
    test_shader_language.cpp
    test_shader.cpp
    test_index_buffer.cpp
    test_vertex_buffer.cpp
    opengl_test.cpp
)
TARGET_LINK_LIBRARIES(opengltest mgegraphics mgecore gmock)
TARGET_IS_TEST(opengltest NEEDSDISPLAY)

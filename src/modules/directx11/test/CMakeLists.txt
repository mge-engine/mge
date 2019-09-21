# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(dx11test
    test_main.cpp
    dx11_test.cpp
    test_render_system.cpp
    test_window.cpp
    test_shader.cpp
    test_index_buffer.cpp
    test_command_list.cpp
    test_error.cpp

    ${CMAKE_SOURCE_DIR}/src/modules/directx11/error.cpp
)
TARGET_LINK_LIBRARIES(dx11test mgegraphics mgecore gmock)
TARGET_IS_TEST(dx11test NEEDSDISPLAY)

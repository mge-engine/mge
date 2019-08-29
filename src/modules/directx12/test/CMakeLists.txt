# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(dx12test
    test_main.cpp
    dx12_test.cpp
    test_render_system.cpp
    test_window.cpp
    test_error.cpp

    ${CMAKE_SOURCE_DIR}/src/modules/directx12/error.cpp
)
TARGET_LINK_LIBRARIES(dx12test mgegraphics mgecore gmock)
TARGET_IS_TEST(dx12test NEEDSDISPLAY)

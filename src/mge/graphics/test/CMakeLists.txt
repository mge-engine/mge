# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(graphicstest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_render_system.cpp
  test_render_context.cpp
  test_rgb_color.cpp
  test_rgba_color.cpp
  test_extent.cpp
  test_window.cpp
  test_hardware_buffer.cpp
  test_shader.cpp
  test_pipeline.cpp
)
TARGET_LINK_LIBRARIES(graphicstest mgegraphics mgecore gmock)
TARGET_IS_TEST(graphicstest)


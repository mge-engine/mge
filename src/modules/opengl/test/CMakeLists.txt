ADD_EXECUTABLE(opengltest
    test_main.cpp
    test_render_system.cpp
    test_window.cpp
    test_shader_language.cpp
    opengl_test.cpp
)
TARGET_LINK_LIBRARIES(opengltest mgegraphics mgecore gmock)
TARGET_IS_TEST(opengltest)

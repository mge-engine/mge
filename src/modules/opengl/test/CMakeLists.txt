ADD_EXECUTABLE(opengltest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_render_system.cpp
    test_window.cpp
    opengl_test.cpp
)
TARGET_LINK_LIBRARIES(opengltest mgegraphics mgecore gmock)
TARGET_IS_TEST(opengltest)

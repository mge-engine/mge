ADD_EXECUTABLE(dx11test
    test_main.cpp
    dx11_test.cpp
    test_render_system.cpp
    test_window.cpp
)
TARGET_LINK_LIBRARIES(dx11test mgegraphics mgecore gmock)
TARGET_IS_TEST(dx11test)

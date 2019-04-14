ADD_EXECUTABLE(opengltest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  opengl_test.cpp
)
TARGET_LINK_LIBRARIES(opengltest mgegraphics mgecore gmock)
TARGET_IS_TEST(opengltest)

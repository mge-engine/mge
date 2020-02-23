ADD_EXECUTABLE(shadertest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_program.cpp
)
TARGET_LINK_LIBRARIES(shadertest mgeshader mgecore gmock)
TARGET_IS_TEST(shadertest)


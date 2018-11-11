ADD_EXECUTABLE(win32test
  ${CMAKE_SOURCE_DIR}/src/test/test_simple_main.cpp
  test_monitor.cpp

  ${CMAKE_SOURCE_DIR}/src/win32/monitor.cpp
)
TARGET_LINK_LIBRARIES(win32test mgecore mgegraphics gmock)
TARGET_IS_TEST(win32test)


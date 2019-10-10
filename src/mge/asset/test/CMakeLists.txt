
ADD_EXECUTABLE(assettest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_untyped_asset.cpp
)
TARGET_LINK_LIBRARIES(assettest mgeasset mgecore gmock)
TARGET_IS_TEST(assettest)


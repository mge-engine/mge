# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(assettest
  ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
  test_asset.cpp
  test_asset_type.cpp
)
TARGET_LINK_LIBRARIES(assettest mgeasset mgecore gmock)
TARGET_IS_TEST(assettest)


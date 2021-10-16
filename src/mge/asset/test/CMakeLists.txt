# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEASSET_TEST_SOURCES
    test_asset_type.cpp
)

MGE_TEST(
    TARGET      test_asset
    SOURCES     ${MGEASSET_TEST_SOURCES}
    LIBRARIES   mgeasset mgecore
)


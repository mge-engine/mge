# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEASSET_TEST_SOURCES
    test_asset_type.cpp
)

MGE_TEST(
    TARGET      test_asset
    SOURCES     ${MGEASSET_TEST_SOURCES}
    LIBRARIES   mgeasset mgecore
)


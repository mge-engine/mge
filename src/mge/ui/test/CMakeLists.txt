# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEUI_TEST_SOURCES
    test_immediate.cpp
)

MGE_TEST(
    TARGET      test_ui
    SOURCES     ${MGEUI_TEST_SOURCES}
    LIBRARIES   mgecore mgeui mgegraphics mgeasset
)

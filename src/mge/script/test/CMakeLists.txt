# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGESCRIPT_TEST_SOURCES
    test_module.cpp
    test_type.cpp
)

MGE_TEST(
    TARGET      test_script
    SOURCES     ${MGESCRIPT_TEST_SOURCES}
    LIBRARIES   mgecore mgescript
)



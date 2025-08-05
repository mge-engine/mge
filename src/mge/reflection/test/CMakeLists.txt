# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEREFLECTION_TEST_SOURCES
    test_module.cpp
)

MGE_TEST(
    TARGET test_reflection
    SOURCES ${MGEREFLECTION_TEST_SOURCES}
    LIBRARIES mgereflection mgecore
)

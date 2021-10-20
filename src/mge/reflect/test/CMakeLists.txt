# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEREFLECT_TEST_SOURCES
    test_type_details.cpp
)

MGE_TEST(
    TARGET      test_reflect
    SOURCES     ${MGEREFLECT_TEST_SOURCES}
    LIBRARIES   mgereflect mgecore
)



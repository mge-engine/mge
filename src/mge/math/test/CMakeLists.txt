# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(MGEMATH_TEST_SOURCES
    test_vec2.cpp
    test_vec3.cpp
)

MGE_TEST(
    TARGET      test_math
    SOURCES     ${MGEMATH_TEST_SOURCES}
    LIBRARIES   mgemath mgecore
)

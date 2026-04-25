# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(MGEMATH_TEST_SOURCES
    test_mat.cpp
    test_quat.cpp
    test_vec2.cpp
    test_vec3.cpp
    test_vec4.cpp
)

MGE_TEST(
    TARGET      test_math
    SOURCES     ${MGEMATH_TEST_SOURCES}
    LIBRARIES   mgemath mgecore
)

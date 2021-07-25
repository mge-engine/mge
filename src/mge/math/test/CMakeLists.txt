# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(MGEMATH_TESTS
    vec2
    vec3
)

FOREACH(TEST_NAME ${MGEMATH_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgemath mgecore
    )
ENDFOREACH()

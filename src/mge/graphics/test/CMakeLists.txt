# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEGRAPHICS_TESTS
    extent
    point
    window
)

FOREACH(TEST_NAME ${MGEGRAPHICS_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgegraphics mgecore
    )
ENDFOREACH()

# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEINPUT_TESTS
)

FOREACH(TEST_NAME ${MGEINPUT_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgecore
    )
ENDFOREACH()


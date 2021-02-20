# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEAPPLICATION_TESTS
    application)

FOREACH(TEST_NAME ${MGEAPPLICATION_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgeapplication mgecore
    )
ENDFOREACH()

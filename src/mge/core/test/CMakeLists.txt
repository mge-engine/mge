# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGECORE_TESTS
    executable_name
    type_name
    log)

FOREACH(TEST_NAME ${MGECORE_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mgecore
    )
ENDFOREACH()

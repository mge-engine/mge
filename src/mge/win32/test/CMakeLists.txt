# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(WIN32_TESTS
    win32_monitor
    win32_window)

FOREACH(TEST_NAME ${WIN32_TESTS})
    MGE_TEST(
        TARGET      test_${TEST_NAME}
        SOURCES     test_${TEST_NAME}.cpp
        LIBRARIES   mge_win32 mgecore mgegraphics mgeapplication mgeinput
    )
ENDFOREACH()

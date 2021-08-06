# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(MGEINPUT_TEST_SOURCES
    test_key.cpp
    test_modifier.cpp
    test_mouse_state.cpp
    test_keyboard_state.cpp
)

MGE_TEST(
    TARGET      test_input
    SOURCES     ${MGEINPUT_TEST_SOURCES}
    LIBRARIES   mgecore mgeinput
)



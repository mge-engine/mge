# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGESCRIPT_TEST_SOURCES
    test_module.cpp
    test_function.cpp
    test_enum.cpp
    test_bool.cpp
    test_pointer.cpp
)

MGE_TEST(
    TARGET      test_script
    SOURCES     ${MGESCRIPT_TEST_SOURCES}
    LIBRARIES   mgecore mgescript
)



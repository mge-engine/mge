# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(PYTHON_TEST_SOURCES
    pythontest.cpp
    test_context.cpp
)

MGE_TEST(
    TARGET      test_python
    SOURCES     ${PYTHON_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgescript mgeapplication mgecore
)


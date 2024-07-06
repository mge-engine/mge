# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEAPPLICATION_TEST_SOURCES
    test_application.cpp
    test_simple_loop.cpp)

MGE_TEST(
    TARGET      test_application
    SOURCES     ${MGEAPPLICATION_TEST_SOURCES}
    LIBRARIES   mgeapplication mgecore
)

MGE_TEST(TARGET test_applicationtest
         SOURCES test_applicationtest.cpp
         NOMAIN
         LIBRARIES mgeapplication mgecore)

MGE_TEST(TARGET test_simple_application
         SOURCES test_simple_application.cpp
         NOMAIN
         LIBRARIES mgeapplication mgecore)
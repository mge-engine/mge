# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGESCRIPT_TEST_SOURCES
    test_module.cpp
    test_traits.cpp
    test_type.cpp
#    test_variable.cpp
   test_function.cpp
   test_proxy.cpp
   test_signature.cpp
)

MGE_TEST(
    TARGET      test_script
    SOURCES     ${MGESCRIPT_TEST_SOURCES}
    LIBRARIES   mgecore mgescript
)



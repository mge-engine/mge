# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEREFLECTION_TEST_SOURCES
    test_function.cpp
    test_module.cpp
    test_signature.cpp
    test_type.cpp
    test_type_identifier.cpp

#    test_type_name.cpp
)

MGE_TEST(
    TARGET test_reflection
    SOURCES ${MGEREFLECTION_TEST_SOURCES}
    LIBRARIES mgereflection mgecore
)

MGE_TEST(
    TARGET test_reflector
    SOURCES test_reflector.cpp
    LIBRARIES mgereflection mgecore
)

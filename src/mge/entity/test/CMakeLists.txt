# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGEENTITY_TEST_SOURCES
    test_registry.cpp
)

MGE_TEST(
    TARGET      test_entity
    SOURCES     ${MGEENTITY_TEST_SOURCES}
    LIBRARIES   mgeentity mgecore
)


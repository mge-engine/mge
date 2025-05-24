# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(MGESCENE_TEST_SOURCES
    test_scene.cpp
)

MGE_TEST(
    TARGET      test_scene
    SOURCES     ${MGESCENE_TEST_SOURCES}
    LIBRARIES   mgecore mgescene
)


# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_EXECUTABLE(scenetest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    test_scene.cpp)
TARGET_LINK_LIBRARIES(scenetest mgescene mgecore gmock)
TARGET_IS_TEST(scenetest)

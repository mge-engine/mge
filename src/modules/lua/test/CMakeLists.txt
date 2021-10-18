# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(LUA_TEST_SOURCES
    luatest.cpp
    test_context.cpp
)

MGE_TEST(
    TARGET      test_lua
    SOURCES     ${LUA_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgescript mgeapplication mgecore
)


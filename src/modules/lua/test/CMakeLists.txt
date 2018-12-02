# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_EXECUTABLE(luatest
    ${CMAKE_SOURCE_DIR}/src/test/test_main.cpp
    lua_test.cpp
    test_context.cpp)
TARGET_LINK_LIBRARIES(luatest mgescript mgecore gmock)
TARGET_IS_TEST(luatest)

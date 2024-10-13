# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(LUA_TEST_SOURCES
    luatest.cpp
    test_context.cpp
)

SET(LUA_TESTS
#    test001_smoke.lua 
)

COPY_FILE(unittest.lua "${CMAKE_BINARY_DIR}/unittest.lua")

LIST(TRANSFORM
    LUA_TESTS
    PREPEND "${CMAKE_CURRENT_BINARY_DIR}/"
    OUTPUT_VARIABLE LUA_TESTS_IN_TARGET)

ADD_CUSTOM_TARGET(all-lua-tests)

FOREACH(LUA_TEST ${LUA_TESTS})
    COPY_FILE(${LUA_TEST} "${CMAKE_CURRENT_BINARY_DIR}/${LUA_TEST}")
    GET_FILENAME_COMPONENT(LUA_TEST_NAME ${LUA_TEST} NAME_WE)
    ADD_TEST(NAME "lua_${LUA_TEST_NAME}"
        COMMAND ${CMAKE_BINARY_DIR}/mgelua.exe "${CMAKE_CURRENT_BINARY_DIR}/${LUA_TEST}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    ADD_CUSTOM_TARGET("lua_${LUA_TEST_NAME}_run" 
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${LUA_TEST}"
        COMMAND ${CMAKE_CTEST_COMMAND} -VV -R "lua_${LUA_TEST_NAME}")
    ADD_DEPENDENCIES(all-lua-tests "lua_${LUA_TEST_NAME}_run")
ENDFOREACH()

ADD_CUSTOM_TARGET(lua-copy-tests ALL DEPENDS
    ${LUA_TESTS_IN_TARGET})

MGE_TEST(
    TARGET      test_lua
    SOURCES     ${LUA_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgescript mgeapplication mgecore
)


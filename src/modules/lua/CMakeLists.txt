# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mge_lua_module_SOURCES
    lua_engine.cpp
    lua_context.cpp
    lua_error.cpp
    lua_bind_helper.cpp)

ADD_LIBRARY(mge_module_lua MODULE
            ${mge_lua_module_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_lua
    mgecore
    mgescript
    lua)

ADD_SUBDIRECTORY(test)
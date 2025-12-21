# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_lua_module_SOURCES
    lua_engine.cpp
    lua_module.cpp
    lua_context.cpp
    lua_binder.cpp
    lua_error.cpp
)

ADD_LIBRARY(mge_module_lua MODULE
            ${mge_lua_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_lua
    mgecore
    mgereflection
    mgescript

    PkgConfig::LUAJIT)

ADD_EXECUTABLE(mgelua mgelua.cpp)
TARGET_LINK_LIBRARIES(mgelua mgecore mgereflection mgescript)
    
ADD_SUBDIRECTORY(test)
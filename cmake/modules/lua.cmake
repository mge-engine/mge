# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

find_package(Lua 5.3 REQUIRED)
ADD_LIBRARY(lua UNKNOWN IMPORTED)
SET_PROPERTY(TARGET lua
             PROPERTY IMPORTED_LOCATION "${LUA_LIBRARY}")
SET_PROPERTY(TARGET lua
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")

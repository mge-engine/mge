# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(Lua 5.4 REQUIRED)
##SET_PROPERTY(TARGET lua
##             PROPERTY IMPORTED_LOCATION "${LUA_LIBRARY}")
SET_PROPERTY(TARGET lua
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")


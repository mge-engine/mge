# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mge_module_lua MODULE
            lua_context.cpp
            lua.cpp)
TARGET_LINK_LIBRARIES(mge_module_lua lua mgecore mgescript)

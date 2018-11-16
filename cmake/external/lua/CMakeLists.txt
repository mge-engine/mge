# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(lua_library)

ADD_LIBRARY(lua
src/lapi.c src/lcode.c src/lctype.c src/ldebug.c src/ldo.c src/ldump.c
src/lfunc.c src/lgc.c src/llex.c src/lmem.c src/lobject.c src/lopcodes.c
src/lparser.c src/lstate.c src/lstring.c src/ltable.c src/ltm.c
src/lundump.c src/lvm.c src/lzio.c src/lauxlib.c src/lbaselib.c
src/lbitlib.c src/lcorolib.c src/ldblib.c src/liolib.c src/lmathlib.c
src/loslib.c src/lstrlib.c src/ltablib.c src/loadlib.c src/linit.c
src/lutf8lib.c)


TARGET_COMPILE_DEFINITIONS(lua PUBLIC LUA_COMPAT_ALL LUA_ANSI)

# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

IF(NOT "${LUA_FOUND}")
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/lua)
    SET(CMAKE_LIST_CONTENT "
        CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
        INCLUDE(ExternalProject)
        EXTERNALPROJECT_ADD(lua
                            PREFIX lua
                            URL https://www.lua.org/ftp/lua-5.3.5.tar.gz
                            CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                            INSTALL_COMMAND \"\"
         )
         EXTERNALPROJECT_ADD_STEP(lua lua_copy_cmake_file
                                  COMMAND \"${CMAKE_COMMAND}\" -E copy ${PROJECT_SOURCE_DIR}/cmake/external/lua/CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/external/lua/lua/src/lua/CMakeLists.txt
                                  DEPENDEES patch
                                  DEPENDERS configure
         )
    ")
    FILE(WRITE ${CMAKE_BINARY_DIR}/external/lua/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/lua
    )
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/lua
    )
    SET(LUA_FOUND "TRUE" CACHE INTERNAL "LUA_FOUND")
ENDIF()
IF(MSVC)
    SET(LUA_LIBRARY     "${CMAKE_BINARY_DIR}/external/lua/lua/src/lua-build/lua.lib")
ELSE()
    SET(LUA_LIBRARY     "${CMAKE_BINARY_DIR}/external/lua/lua/src/lua-build/liblua.a")
ENDIF()
SET(LUA_INCLUDE_DIR "${CMAKE_BINARY_DIR}/external/lua/lua/src/lua/src")

ADD_LIBRARY(lua UNKNOWN IMPORTED)
SET_PROPERTY(TARGET lua
             PROPERTY IMPORTED_LOCATION "${LUA_LIBRARY}")
SET_PROPERTY(TARGET lua
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}")

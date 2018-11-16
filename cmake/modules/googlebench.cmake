# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

IF(NOT "${Googlebenchmark_FOUND}")
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/googlebenchmark)
    SET(CMAKE_LIST_CONTENT "
        CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
        INCLUDE(ExternalProject)
        EXTERNALPROJECT_ADD(googlebenchmark
                        PREFIX googlebenchmark
                        URL https://github.com/google/benchmark/archive/v1.2.0.tar.gz
                        CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                         INSTALL_COMMAND \"\"
         )
    ")
    FILE(WRITE ${CMAKE_BINARY_DIR}/external/googlebenchmark/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/googlebenchmark
    )
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/googlebenchmark
    )
    SET(Googlebenchmark_FOUND "TRUE" CACHE INTERNAL "Googlebenchmark_FOUND")
ENDIF()
IF(MSVC)
    SET(Googlebenchmark_LIBRARY     "${CMAKE_BINARY_DIR}/external/googlebenchmark/googlebenchmark/src/googlebenchmark-build/src/benchmark.lib")
ELSE()
    SET(Googlebenchmark_LIBRARY     "${CMAKE_BINARY_DIR}/external/googlebenchmark/googlebenchmark/src/googlebenchmark-build/src/libbenchmark.a")
ENDIF()
SET(Googlebenchmark_INCLUDE_DIR "${CMAKE_BINARY_DIR}/external/googlebenchmark/googlebenchmark/src/googlebenchmark/include")

ADD_LIBRARY(googlebenchmark UNKNOWN IMPORTED)
SET_PROPERTY(TARGET googlebenchmark
             PROPERTY IMPORTED_LOCATION "${Googlebenchmark_LIBRARY}")
SET_PROPERTY(TARGET googlebenchmark
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Googlebenchmark_INCLUDE_DIR}")
SET_PROPERTY(TARGET googlebenchmark
                PROPERTY INTERFACE_LINK_LIBRARIES shlwapi)



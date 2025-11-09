# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/linenoise-ng)
SET(CMAKE_LIST_CONTENT "
    CMAKE_MINIMUM_REQUIRED(VERSION 3.27)
    PROJECT(LINENOISENG)
    INCLUDE(ExternalProject)
    EXTERNALPROJECT_ADD(linenoise-ng-external
                        PREFIX linenoise
                        GIT_REPOSITORY \"https://github.com/mge-engine/linenoise-ng.git\"
                        GIT_TAG \"55ee4c3f393ca4912acf879b13def71c5fc23065\"
                        INSTALL_COMMAND \"\"
                        BUILD_COMMAND \"\"
                        CONFIGURE_COMMAND \"\"
                        BUILD_IN_SOURCE TRUE

    )
")
FILE(WRITE ${CMAKE_BINARY_DIR}/external/linenoise-ng/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/linenoise-ng
)
EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
                WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/linenoise-ng
)

SET(__linenoise_source_dir ${CMAKE_BINARY_DIR}/external/linenoise-ng/linenoise/src/linenoise-ng-external/src)

SET(linenoise_SOURCES
    ${__linenoise_source_dir}/linenoise.cpp
    ${__linenoise_source_dir}/ConvertUTF.cpp
    ${__linenoise_source_dir}/wcwidth.cpp
)

ADD_LIBRARY(linenoise
            STATIC
            ${linenoise_SOURCES})
TARGET_INCLUDE_DIRECTORIES(linenoise PUBLIC ${CMAKE_BINARY_DIR}/external/linenoise-ng/linenoise/src/linenoise-ng-external/include)
SET_TARGET_PROPERTIES(linenoise PROPERTIES POSITION_INDEPENDENT_CODE ON)
IF(MSVC)
    TARGET_COMPILE_OPTIONS(linenoise PRIVATE -wd4477 -wd5052 -wd4244 -wd4242 -wd4245 -wd4505 -wd4701)
ENDIF()



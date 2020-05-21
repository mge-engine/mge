# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(OpenGL)

IF(OpenGL_FOUND)
    FIND_PACKAGE(PythonInterp)
    IF(NOT PYTHONINTERP_FOUND)
        MESSAGE(FATAL_ERROR "A python interpreter is needed to build the gl3w layer")
    ENDIF()
    IF(NOT GL3W_FOUND)
        MESSAGE("-- Did not find gl3w, need to build it from source")
        FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/gl3w)
        SET(CMAKE_LIST_CONTENT "
            PROJECT(GL3W)
            CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
            INCLUDE(ExternalProject)
            EXTERNALPROJECT_ADD(gl3w-external
                                PREFIX gl3w
                                GIT_REPOSITORY \"https://github.com/skaslev/gl3w.git\"
                                GIT_TAG 4f1d558410b0938840dc3db98e741d71f382ba22
                                INSTALL_COMMAND \"\"
                                BUILD_IN_SOURCE TRUE
            )
        ")
        FILE(WRITE ${CMAKE_BINARY_DIR}/external/gl3w/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                         WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/gl3w
        )
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
                         WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/gl3w
        )
        SET(GL3W_FOUND "TRUE" CACHE INTERNAL "GL3W_FOUND")
    ENDIF()
    ADD_LIBRARY(gl3w STATIC
        ${CMAKE_BINARY_DIR}/external/gl3w/gl3w/src/gl3w-external/src/gl3w.c)
    TARGET_INCLUDE_DIRECTORIES(gl3w PUBLIC
        "${CMAKE_BINARY_DIR}/external/gl3w/gl3w/src/gl3w-external/include")
    IF(MSVC)
        TARGET_COMPILE_OPTIONS(gl3w PRIVATE "-wd4152")
    ENDIF()
ENDIF()

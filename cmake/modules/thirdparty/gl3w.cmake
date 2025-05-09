# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
INCLUDE(thirdparty/opengl)
    
IF(OpenGL_FOUND)
    FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/gl3w)
    SET(CMAKE_LIST_CONTENT "
        cmake_minimum_required(VERSION 4.0)
        PROJECT(GL3W)
        INCLUDE(ExternalProject)
        EXTERNALPROJECT_ADD(gl3w-external
                            PREFIX gl3w
                            GIT_REPOSITORY \"https://github.com/mge-engine/gl3w.git\"
                            GIT_TAG \"e90a19dc269689b17097fabc9e3eebeb5dc5e42f\"
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
    ADD_LIBRARY(gl3w STATIC
        ${CMAKE_BINARY_DIR}/external/gl3w/gl3w/src/gl3w-external/src/gl3w.c)
    IF(MSVC)
        TARGET_COMPILE_OPTIONS(gl3w PRIVATE "-wd4191") # unsafe pointer conversion)
    ENDIF()
    TARGET_INCLUDE_DIRECTORIES(gl3w PUBLIC
        "${CMAKE_BINARY_DIR}/external/gl3w/gl3w/src/gl3w-external/include")
    IF(MSVC)
        TARGET_COMPILE_OPTIONS(gl3w PRIVATE "-wd4152")
    ENDIF()
ENDIF()

# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Boost)
IF(NOT "${Boost_FOUND}")
    IF(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/src/boost")
        MESSAGE("Need to download and install boost, not found on the system")
        FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/boost)
        SET(CMAKE_LIST_CONTENT "
            CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
            INCLUDE(ExternalProject)
            EXTERNALPROJECT_ADD(boost
                            PREFIX boost
                            URL https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.zip
                            CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                            INSTALL_COMMAND \"\"
            )
            EXTERNALPROJECT_ADD_STEP(boost boost_copy_cmake_file
                                     COMMAND \"${CMAKE_COMMAND}\" -E copy ${PROJECT_SOURCE_DIR}/cmake/external/boost/CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/src/boost/CMakeLists.txt
                                     DEPENDEES patch
                                     DEPENDERS configure
            )
        ")
        FILE(WRITE ${CMAKE_BINARY_DIR}/external/boost/CMakeLists.txt "${CMAKE_LIST_CONTENT}")
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/boost
        )
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
                        WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/boost
        )
    ENDIF()
    ADD_LIBRARY(boost INTERFACE)
    SET_PROPERTY(TARGET boost
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/external/boost/boost/src/boost")
ELSE()
    ADD_LIBRARY(boost INTERFACE)
    SET_PROPERTY(TARGET boost
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${boost_INCLUDE_DIRS}")
ENDIF()

# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Sqlite3)
IF(NOT "${Sqlite3_FOUND}")
    MESSAGE("-- Retry again with build location ...")
    SET(Sqlite3_INCLUDE_DIR "${PROJECT_BINARY_DIR}/external/sqlite3/sqlite3/src/sqlite3")
    SET(Sqlite3_LIBRARY_DIR "${PROJECT_BINARY_DIR}/external/sqlite3/sqlite3/src/sqlite3-build")
    FIND_PACKAGE(Sqlite3)
    IF(NOT "${Sqlite3_FOUND}")
        MESSAGE("Need to build Sqlite3, not found in the system")
        FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external ${CMAKE_BINARY_DIR}/external/sqlite3)
        SET(CMAKE_LIST_CONTENT "
            CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
            INCLUDE(ExternalProject)
            EXTERNALPROJECT_ADD(sqlite3
                            PREFIX sqlite3
                            URL https://www.sqlite.org/2017/sqlite-amalgamation-3200000.zip
                            CMAKE_ARGS -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
                            INSTALL_COMMAND \"\"
            )
            EXTERNALPROJECT_ADD_STEP(sqlite3 sqlite3_copy_cmake_file
                                     COMMAND \"${CMAKE_COMMAND}\" -E copy ${PROJECT_SOURCE_DIR}/cmake/external/sqlite3/CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/external/sqlite3/sqlite3/src/sqlite3/CMakeLists.txt
                                     DEPENDEES patch
                                     DEPENDERS configure
            )
        ")
        FILE(WRITE ${CMAKE_BINARY_DIR}/external/sqlite3/CMakeLists.txt "${CMAKE_LIST_CONTENT}")

        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" . -G${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/sqlite3
            )
        EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY  ${CMAKE_BINARY_DIR}/external/sqlite3
            )
        MESSAGE("-- Retry again with build location ...")
        SET(Sqlite3_INCLUDE_DIR "${PROJECT_BINARY_DIR}/external/sqlite3/sqlite3/src/sqlite3")
        SET(Sqlite3_LIBRARY_DIR "${PROJECT_BINARY_DIR}/external/sqlite3/sqlite3/src/sqlite3-build")
        FIND_PACKAGE(Sqlite3)
    ELSE()
        MESSAGE("-- Sqlite3 found from previous build")
    ENDIF()
ENDIF()

IF(NOT "${Sqlite3_FOUND}")
    MESSAGE(FATAL "Cannot find sqlite3 in the system and self-built version failed")
ENDIF()

ADD_LIBRARY(sqlite3 UNKNOWN IMPORTED)
SET_PROPERTY(TARGET sqlite3
             PROPERTY IMPORTED_LOCATION "${Sqlite3_LIBRARY_RELEASE}")
SET_PROPERTY(TARGET sqlite3
             PROPERTY IMPORTED_LOCATION_RELEASE "${Sqlite3_LIBRARY_RELEASE}")
SET_PROPERTY(TARGET sqlite3
             PROPERTY IMPORTED_LOCATION_DEBUG "${Sqlite3_LIBRARY_DEBUG}")
SET_PROPERTY(TARGET sqlite3
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Sqlite3_INCLUDE_DIR}")
ADD_EXECUTABLE(mgesqlite IMPORTED GLOBAL)
SET_PROPERTY(TARGET mgesqlite
             PROPERTY IMPORTED_LOCATION "${Sqlite3_LIBRARY_RELEASE}")


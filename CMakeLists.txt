# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

CMAKE_MINIMUM_REQUIRED(VERSION 3.29.2)
PROJECT(MGE)


IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    FILE(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/CMakeFiles")
    MESSAGE(FATAL_ERROR "In-source builds are not supported")
ENDIF()

# Global settings
# - enable testing
# - enable exporting compile_commands.json (for editors etc.)
ENABLE_TESTING()
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MGE_VERSION_MAJOR 0)
SET(MGE_VERSION_MINOR 1)
SET(MGE_PATCHLEVEL 0)

# Set MGE_BUILD to the current git short hash
EXECUTE_PROCESS(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE MGE_BUILD
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
    RESULT_VARIABLE GIT_RESULT
)

# If git command failed, set a default value
IF(NOT GIT_RESULT EQUAL 0)
    SET(MGE_BUILD "unknown")
ENDIF()

# Check if repository has uncommitted changes
EXECUTE_PROCESS(
    COMMAND git status --porcelain
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_STATUS_OUTPUT
    ERROR_QUIET
    RESULT_VARIABLE GIT_STATUS_RESULT
)

# Set MGE_DIRTY to TRUE if there are uncommitted changes
IF(GIT_STATUS_RESULT EQUAL 0 AND NOT "${GIT_STATUS_OUTPUT}" STREQUAL "")
    SET(MGE_CLEAN_BUILD 0)
ELSE()
    SET(MGE_CLEAN_BUILD 1)
ENDIF()

SET(MGE_VERSION_FULL "${MGE_VERSION_MAJOR}.${MGE_VERSION_MINOR}.${MGE_PATCHLEVEL}")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Output libraries and binaries into the main build folder
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include directories
# both in source and in build directory for generated includes
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/"
                    "${CMAKE_BINARY_DIR}/src/")

INCLUDE(setup)

ADD_SUBDIRECTORY(assets)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(docsrc)
ADD_SUBDIRECTORY(doc)
ADD_SUBDIRECTORY(tools)



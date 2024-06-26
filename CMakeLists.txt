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


SET(MGE_VERSION_MAJOR 0)
SET(MGE_VERSION_MINOR 1)
SET(MGE_PATCHLEVEL 0)
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



# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

CMAKE_MINIMUM_REQUIRED(VERSION 3.9.0)
PROJECT(MGE)
SET(MGE_VERSION_MAJOR 0)
SET(MGE_VERSION_MINOR 1)
SET(MGE_PATCHLEVEL 0)
SET(MGE_VERSION_FULL "${MGE_VERSION_MAJOR}.${MGE_VERSION_MINOR}.${MGE_PATCHLEVEL}")

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Output libraries and binaries into the main build folder
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Include directories
# - src     for private includes
# - include for public includes
# both in source and in build directory for generated includes
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/"
                    "${CMAKE_BINARY_DIR}/src/"
                    "${CMAKE_SOURCE_DIR}/include/"
                    "${CMAKE_BINARY_DIR}/include/")

ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)

# Global settings
# - enable testing
# - enable exporting compile_commands.json (for editors etc.)
ENABLE_TESTING()
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)



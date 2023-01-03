# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

# Python is both used as tool and as component, avoid
# multiple includes.
INCLUDE_GUARD(GLOBAL)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
IF(NOT Python3_FOUND)
    MESSAGE(FATAL_ERROR "A python interpreter is needed to build mge")
ELSE()
    MESSAGE("-- Python interpreter version ${Python3_VERSION} found")
    MESSAGE("-- Python interpreter used: ${Python3_EXECUTABLE}")
ENDIF()

IF(Python3_Development_FOUND)
    MESSAGE("-- Python development tools have been found")
    ADD_LIBRARY(python3 ALIAS Python3::Python)
    GET_FILENAME_COMPONENT(_python3path ${Python3_EXECUTABLE} DIRECTORY)
    MESSAGE("-- Copying python installation to target")
    FILE(COPY ${_python3path} DESTINATION ${CMAKE_BINARY_DIR})
ENDIF()

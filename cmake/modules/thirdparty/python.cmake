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

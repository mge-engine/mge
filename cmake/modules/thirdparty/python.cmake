# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

# Python is both used as tool and as component, avoid
# multiple includes.
INCLUDE_GUARD(GLOBAL)

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
IF(NOT Python3_FOUND)
    MESSAGE(FATAL_ERROR "A python interpreter is needed to build mge")
ELSE()
    MESSAGE("Python interpreter version ${Python3_VERSION} found")
ENDIF()

IF(Python3_Development_FOUND)
    MESSAGE("Python development tools have been found")
    ADD_LIBRARY(python3 UNKNOWN IMPORTED)
    SET_PROPERTY(TARGET python3
      PROPERTY IMPORTED_LOCATION "${Python3_LIBRARIES}")
    SET_PROPERTY(TARGET python3
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Python3_INCLUDE_DIRS}")
ENDIF()
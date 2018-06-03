# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(PythonInterp)

IF(NOT MINGW)
  FIND_PACKAGE(PythonLibs)
ENDIF()

IF(PYTHONINTERP_FOUND)
    MESSAGE(STATUS "Python interpreter version ${PYTHON_VERSION_STRING} found")
ELSE()
    MESSAGE(FATAL_ERROR "A python interpreter is needed to compile moge")
ENDIF()

IF(PYTHONLIBS_FOUND)
ADD_LIBRARY(python UNKNOWN IMPORTED)
SET_PROPERTY(TARGET python
             PROPERTY IMPORTED_LOCATION "${PYTHON_LIBRARIES}")
SET_PROPERTY(TARGET python
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PYTHON_INCLUDE_DIRS}")
ENDIF()

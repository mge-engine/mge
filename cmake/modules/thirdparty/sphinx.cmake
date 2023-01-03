# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PROGRAM(SPHINX_EXECUTABLE
             NAMES sphinx-build
             DOC "Path to sphinx-build executable")
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sphinx
                                  "Cannot find sphinx-build executable"
                                  SPHINX_EXECUTABLE)

IF (SPHINX_FOUND)
    MESSAGE("-- Sphinx documentation generator was found")
ENDIF()
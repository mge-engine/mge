# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
IF(NOT Python3_FOUND)
    MESSAGE(FATAL_ERROR "A python interpreter is needed to build mge")
ENDIF()


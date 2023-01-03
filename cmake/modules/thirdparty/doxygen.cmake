# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(Doxygen)

IF(NOT Doxygen_FOUND)
    MESSAGE("-- Doxygen not found, not building documentation")
ENDIF()
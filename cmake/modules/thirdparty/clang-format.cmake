# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(ClangFormat)

IF(CLANG_FORMAT_FOUND)
    MESSAGE("clang-format executable found: ${CLANG_FORMAT_EXECUTABLE} version: ${CLANG_FORMAT_VERSION}")
ENDIF()
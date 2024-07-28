# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FIND_LIBRARY(JEMALLOC_LIB
    NAMES jemalloc
    PATHS ${CMAKE_LIBRARY_PATHS}
)

FIND_PATH(JEMALLOC_INCLUDE_DIR jemalloc/jemalloc.h PATHS ${VCPKG_CMAKE_FIND_ROOT_PATH})

IF(JEMALLOC_INCLUDE_DIR)
    MESSAGE(STATUS "Found jemalloc include directory: ${JEMALLOC_INCLUDE_DIR}")
ELSE()
    MESSAGE(FATAL_ERROR "Internal error: could not find jemalloc/jemalloc.h")
ENDIF()

ADD_LIBRARY(jemalloc::jemalloc INTERFACE IMPORTED)
TARGET_INCLUDE_DIRECTORIES(jemalloc::jemalloc INTERFACE ${JEMALLOC_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(jemalloc::jemalloc INTERFACE ${JEMALLOC_LIB})



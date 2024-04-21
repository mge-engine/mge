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
    GET_FILENAME_COMPONENT(_python3path ${Python3_EXECUTABLE} DIRECTORY)
    MESSAGE("-- Copying python installation to target")
    FILE(GLOB_RECURSE _python3files RELATIVE "${_python3path}" "${_python3path}/*")
    FOREACH(_file ${_python3files})
        GET_FILENAME_COMPONENT(_file_base ${_file} NAME)
        IF(NOT _file_base MATCHES ".*\\.pyc")
            MESSAGE("-- Copying ${_file} to ${CMAKE_BINARY_DIR}/python3/${_file}")
            CONFIGURE_FILE("${_python3path}/${_file}" ${CMAKE_BINARY_DIR}/python3/${_file} COPYONLY)
        ENDIF()
    ENDFOREACH()
ENDIF()

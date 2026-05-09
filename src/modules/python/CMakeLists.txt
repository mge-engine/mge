# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
SET(mge_python_module_SOURCES
    python_engine.cpp
    python_context.cpp
    python_error.cpp
    python_binder.cpp
    python_module.cpp
)

IF(Python3_Development_FOUND)
    MESSAGE("-- Python development tools have been found")
    GET_FILENAME_COMPONENT(_python3path ${Python3_EXECUTABLE} DIRECTORY)
    MESSAGE("-- Copying python installation to target")
    FILE(GLOB_RECURSE _python3files RELATIVE "${_python3path}" "${_python3path}/*")
    FOREACH(_file ${_python3files})
        GET_FILENAME_COMPONENT(_file_base ${_file} NAME)
        IF(NOT _file_base MATCHES ".*\\.pyc")
            CONFIGURE_FILE("${_python3path}/${_file}" ${CMAKE_BINARY_DIR}/python3/${_file} COPYONLY)
        ENDIF()
    ENDFOREACH()
ENDIF()


ADD_LIBRARY(mge_module_python MODULE
            ${mge_python_module_SOURCES})

IF(WIN32)
    TARGET_LINK_LIBRARIES(mge_module_python
        mgecore
        mgereflection
        mgescript
        Python3::Python
        psapi)
ELSE()
    TARGET_LINK_LIBRARIES(mge_module_python
        mgecore
        mgereflection
        mgescript
        Python3::Python)
ENDIF()

ADD_EXECUTABLE(mgepython mgepython.cpp)
TARGET_LINK_LIBRARIES(mgepython mgecore mgescript)


ADD_SUBDIRECTORY(test)
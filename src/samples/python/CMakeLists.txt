# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(Python3_Development_FOUND)
    FILE(GLOB_RECURSE _python_samples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.py)
    FOREACH(_sample ${_python_samples})
        MESSAGE(STATUS "Copying python sample ${_sample} to ${CMAKE_BINARY_DIR}/samples/python")
        CONFIGURE_FILE(${_sample} ${CMAKE_BINARY_DIR}/samples/python/${_sample} COPYONLY)
    ENDFOREACH()
ENDIF()



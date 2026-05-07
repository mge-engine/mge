# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
SET(mge_python_module_SOURCES
    python_engine.cpp
    python_context.cpp
    python_error.cpp
)

SET(MGE_PYTHON_VENV_DIR "${CMAKE_BINARY_DIR}/python3")
IF(NOT EXISTS "${MGE_PYTHON_VENV_DIR}")
    MESSAGE(STATUS "Creating Python venv at ${MGE_PYTHON_VENV_DIR}")
    EXECUTE_PROCESS(
        COMMAND "${Python3_EXECUTABLE}" -m venv "${MGE_PYTHON_VENV_DIR}"
        RESULT_VARIABLE _venv_result
    )
    IF(NOT _venv_result EQUAL 0)
        MESSAGE(FATAL_ERROR "Failed to create Python venv at ${MGE_PYTHON_VENV_DIR}")
    ENDIF()
ENDIF()

ADD_LIBRARY(mge_module_python MODULE
            ${mge_python_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_python
    mgecore
    mgereflection
    mgescript
    Python3::Python)

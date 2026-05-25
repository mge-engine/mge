# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
SET(mge_python_module_SOURCES
    python_engine.cpp
    python_context.cpp
    python_error.cpp
    python_binder.cpp
    python_call_context.cpp
    python_invocation_context.cpp
    python_module.cpp
    python_type.cpp
)

IF(Python3_Development_FOUND)
    MESSAGE("-- Python development tools have been found")
    EXECUTE_PROCESS(
        COMMAND ${Python3_EXECUTABLE} -c "import sys; print(sys.base_prefix)"
        OUTPUT_VARIABLE _python3path
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE _python3baseprefix_result
    )
    IF(NOT _python3baseprefix_result EQUAL 0 OR NOT IS_DIRECTORY "${_python3path}")
        MESSAGE(WARNING "Unable to determine Python base prefix, falling back to executable directory")
        GET_FILENAME_COMPONENT(_python3path ${Python3_EXECUTABLE} DIRECTORY)
    ENDIF()
    MESSAGE("-- Python runtime source: ${_python3path}")
    MESSAGE("-- Scheduling python installation copy to build time")

    ADD_CUSTOM_TARGET(copy-python3-runtime ALL
        COMMAND ${CMAKE_COMMAND}
            -DSRC_DIR="${_python3path}"
            -DDST_DIR="${CMAKE_BINARY_DIR}/python3"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/copy_python3.cmake"
        COMMENT "Copying Python installation to ${CMAKE_BINARY_DIR}/python3"
    )
ENDIF()


ADD_LIBRARY(mge_module_python MODULE
            ${mge_python_module_SOURCES})

IF(Python3_Development_FOUND)
    ADD_DEPENDENCIES(mge_module_python copy-python3-runtime)
ENDIF()

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
TARGET_LINK_LIBRARIES(mgepython mgecore mgescript mgereflection)


ADD_SUBDIRECTORY(test)
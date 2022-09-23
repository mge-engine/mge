# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(PYTHON_TEST_SOURCES
    pythontest.cpp
    test_context.cpp
)

SET(PYTHON_TESTS
    test001_smoke.py
    test002_inherit.py)

LIST(TRANSFORM
    PYTHON_TESTS
    PREPEND "${CMAKE_CURRENT_BINARY_DIR}/"
    OUTPUT_VARIABLE PYTHON_TESTS_IN_TARGET)

FOREACH(PYTHON_TEST ${PYTHON_TESTS})
    COPY_FILE(${PYTHON_TEST} "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}")
    GET_FILENAME_COMPONENT(PYTHON_TEST_NAME ${PYTHON_TEST} NAME_WE)
    ADD_TEST(NAME "python_${PYTHON_TEST_NAME}"
        COMMAND ${CMAKE_BINARY_DIR}/mgepython.exe -m unittest "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
ENDFOREACH()

ADD_CUSTOM_TARGET(python-copy-tests ALL DEPENDS
    ${PYTHON_TESTS_IN_TARGET})

MGE_TEST(
    TARGET      test_python
    SOURCES     ${PYTHON_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgescript mgeapplication mgecore
)


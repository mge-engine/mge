# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(PYTHON_TEST_SOURCES
    pythontest.cpp
    test_context.cpp
)

SET(PYTHON_TESTS
    test001_basic.py
    test002_enum.py
    test003_object.py 
    test004_function.py
    test007_component.py)

# Only include render system test if not in headless environment
if(NOT HEADLESS_ENVIRONMENT)
    list(APPEND PYTHON_TESTS test006_render_system.py)
endif()

LIST(TRANSFORM
    PYTHON_TESTS
    PREPEND "${CMAKE_CURRENT_BINARY_DIR}/"
    OUTPUT_VARIABLE PYTHON_TESTS_IN_TARGET)


ADD_CUSTOM_TARGET(all-python-tests)

FOREACH(PYTHON_TEST ${PYTHON_TESTS})
    COPY_FILE(${PYTHON_TEST} "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}")
    GET_FILENAME_COMPONENT(PYTHON_TEST_NAME ${PYTHON_TEST} NAME_WE)
    ADD_TEST(NAME "python_${PYTHON_TEST_NAME}"
        COMMAND ${CMAKE_BINARY_DIR}/mgepython.exe -m unittest "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
    ADD_CUSTOM_TARGET("python_${PYTHON_TEST_NAME}_run" 
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_TEST}"
        COMMAND ${CMAKE_CTEST_COMMAND} -VV -R "python_${PYTHON_TEST_NAME}")
    ADD_DEPENDENCIES(all-python-tests "python_${PYTHON_TEST_NAME}_run")
ENDFOREACH()

ADD_CUSTOM_TARGET(python-copy-tests ALL DEPENDS
    ${PYTHON_TESTS_IN_TARGET})

MGE_TEST(
    TARGET      test_python
    SOURCES     ${PYTHON_TEST_SOURCES}
    NOMAIN
    LIBRARIES   mgescript mgeapplication mgecore
)


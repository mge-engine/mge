# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

ENABLE_TESTING()

FUNCTION(MGE_TEST)
    # -- argument parsing
    SET(OPTIONS NEEDSDISPLAY NOMAIN)
    SET(ONE_VALUE_ARGS TARGET)
    SET(MULTI_VALUE_ARGS SOURCES LIBRARIES)
    CMAKE_PARSE_ARGUMENTS(MGE_TEST "${OPTIONS}" "${ONE_VALUE_ARGS}"
        "${MULTI_VALUE_ARGS}" "${ARGN}")

    IF(NOT DEFINED MGE_TEST_TARGET)
        MESSAGE(FATAL_ERROR "TARGET missing in call to MGE_TEST")
    ENDIF()
    IF(NOT DEFINED MGE_TEST_SOURCES)
        MESSAGE(FATAL_ERROR "SOURCES missing in call to MGE_TEST for target ${MGE_TEST_TARGET}")
    ENDIF()

    IF(NOT MGE_TEST_NOMAIN)
        LIST(APPEND MGE_TEST_SOURCES
             "${CMAKE_SOURCE_DIR}/src/test/test_main.cpp")
        LIST(APPEND MGE_TEST_LIBRARIES
             googlemock)
    ENDIF()
    # -- build test executable
    ADD_EXECUTABLE("${MGE_TEST_TARGET}"
        "${MGE_TEST_SOURCES}")
    TARGET_LINK_LIBRARIES("${MGE_TEST_TARGET}" "${MGE_TEST_LIBRARIES}" )
    # -- add target as test
    GET_FILENAME_COMPONENT(_PLAIN_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM}" NAME)
    IF("${_PLAIN_MAKE_PROGRAM}" STREQUAL "MSBuild.exe")
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    ELSE()
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}")
    ENDIF()
    ADD_TEST(NAME ${MGE_TEST_TARGET}
             COMMAND ${_BINARY_DIR}/${MGE_TEST_TARGET} --gtest_output=xml:${MGE_TEST_TARGET}.xml
             WORKING_DIRECTORY "${_BINARY_DIR}")
ENDFUNCTION()

FUNCTION(MGE_SIMPLE_TEST)
    SET(OPTIONS NEEDSDISPLAY)
    SET(ONE_VALUE_ARGS TARGET)
    SET(MULTI_VALUE_ARGS SOURCES LIBRARIES)

    CMAKE_PARSE_ARGUMENTS(MGE_SIMPLE_TEST "${OPTIONS}" "${ONE_VALUE_ARGS}"
        "${MULTI_VALUE_ARGS}" "${ARGN}")
    IF(NOT DEFINED MGE_SIMPLE_TEST_TARGET)
        MESSAGE(FATAL_ERROR "TARGET missing in call to MGE_TEST")
    ENDIF()
    SET(_MGE_SIMPLE_TEST_SOURCE "${MGE_SIMPLE_TEST_TARGET}.cpp")
    IF(NOT EXISTS "${_MGE_SIMPLE_TEST_SOURCE}")
        MESSAGE(FATAL_ERROR "Source file ${_MGE_SIMPLE_TEST_SOURCE} not found")
    ENDIF()
    IF(${NEEDSDISPLAY})
        MGE_TEST(TARGET ${MGE_SIMPLE_TEST_TARGET}
                 SOURCES "${_MGE_SIMPLE_TEST_SOURCE}"
                 LIBRARIES "${MGE_SIMPLE_TEST_LIBRARIES}"
                 NEEDSDISPLAY)
    ELSE()
        MGE_TEST(TARGET ${MGE_SIMPLE_TEST_TARGET}
                 SOURCES "${_MGE_SIMPLE_TEST_SOURCE}"
                 LIBRARIES "${MGE_SIMPLE_TEST_LIBRARIES}")
    ENDIF()
ENDFUNCTION()
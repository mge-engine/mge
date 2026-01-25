# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ENABLE_TESTING()
INCLUDE(thirdparty/python)
INCLUDE(thirdparty/renderdoc)
INCLUDE(thirdparty/opengl)
INCLUDE(thirdparty/vulkan)
INCLUDE(thirdparty/directx)

IF (HEADLESS_ENVIRONMENT)
    MESSAGE("-- Headless environment detected, skipping capture tests")
    SET(MGE_NO_CAPTURE_TESTS TRUE)
ELSE()
    IF (NOT RENDERDOCCMD_EXECUTABLE)
        MESSAGE("-- RenderDoc command line tool not found, skipping capture tests")
        SET(MGE_NO_CAPTURE_TESTS TRUE)
    ELSE()
        IF(OpenGL_FOUND)
            SET(MGE_RENDER_SYSTEMS "opengl")
        ENDIF()
        IF(Vulkan_FOUND)
            LIST(APPEND MGE_RENDER_SYSTEMS "vulkan")
        ENDIF()

        IF(DirectX_D3D11_FOUND)
            LIST(APPEND MGE_RENDER_SYSTEMS "directx11")
        ENDIF()

        IF(DirectX_D3D12_FOUND)
            LIST(APPEND MGE_RENDER_SYSTEMS "directx12")
        ENDIF()
    ENDIF()
    IF (NOT MGE_RENDER_SYSTEMS)
        MESSAGE("-- No render systems available, skipping capture tests")
        SET(MGE_NO_CAPTURE_TESTS TRUE)  
    ENDIF()
    MESSAGE("-- Available render systems for tests: ${MGE_RENDER_SYSTEMS}")
ENDIF()

FUNCTION(MGE_TEST)
    # -- argument parsing
    SET(OPTIONS NEEDSDISPLAY NOMAIN SHOWTRACE DISABLED)
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
    ENDIF()
    LIST(APPEND MGE_TEST_LIBRARIES
         googletest googlemock)
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
    # building the test works in all environments, but execution may not
    # happen in headless environments for e.g. graphics tests
    IF(MGE_TEST_DISABLED)
        MESSAGE("-- Skipping test ${MGE_TEST_TARGET} due to DISABLED flag")
    ELSE()
        IF(MGE_TEST_NEEDSDISPLAY)
            IF(HEADLESS_ENVIRONMENT)
                MESSAGE("-- Skipping test ${MGE_TEST_TARGET} due to headless environment")
            ELSE()
                ADD_TEST(NAME ${MGE_TEST_TARGET}
                        COMMAND ${_BINARY_DIR}/${MGE_TEST_TARGET} --gtest_output=xml:${MGE_TEST_TARGET}.xml
                        WORKING_DIRECTORY "${_BINARY_DIR}")
            ENDIF()
        ELSE()
            ADD_TEST(NAME ${MGE_TEST_TARGET}
                    COMMAND ${_BINARY_DIR}/${MGE_TEST_TARGET} --gtest_output=xml:${MGE_TEST_TARGET}.xml
                    WORKING_DIRECTORY "${_BINARY_DIR}")
        ENDIF()
        IF(MGE_TEST_SHOWTRACE)
            SET_PROPERTY(TEST ${MGE_TEST_TARGET}
                        PROPERTY ENVIRONMENT "MGE_TRACE_ENABLED=1")
            SET_PROPERTY(TEST ${MGE_TEST_TARGET}
                        APPEND PROPERTY ENVIRONMENT "MGE_TRACE_TO_STDOUT=1")
        ENDIF()
    ENDIF()
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

FUNCTION(MGE_CAPTURE_TEST)
    # SET(OPTIONS)
    SET(ONE_VALUE_ARGS TARGET)
    #SET(MULTI_VALUE_ARGS SOURCES LIBRARIES)
    CMAKE_PARSE_ARGUMENTS(MGE_CAPTURE_TEST "${OPTIONS}" "${ONE_VALUE_ARGS}"
        "${MULTI_VALUE_ARGS}" "${ARGN}")
    IF(NOT DEFINED MGE_CAPTURE_TEST_TARGET)
        MESSAGE(FATAL_ERROR "TARGET missing in call to MGE_CAPTURE_TEST")
    ENDIF()
    GET_FILENAME_COMPONENT(_PLAIN_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM}" NAME)
    IF("${_PLAIN_MAKE_PROGRAM}" STREQUAL "MSBuild.exe")
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    ELSE()
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}")
    ENDIF()
    IF(NOT MGE_NO_CAPTURE_TESTS)
        FOREACH(RENDER_SYSTEM ${MGE_RENDER_SYSTEMS})
            ADD_TEST(
                NAME test_${MGE_CAPTURE_TEST_TARGET}_capture_${RENDER_SYSTEM}
                COMMAND "${Python3_EXECUTABLE}" 
                    "${CMAKE_BINARY_DIR}/capturetest.py"
                    --test "$<TARGET_FILE:${MGE_CAPTURE_TEST_TARGET}>"
                    --renderdoccmd "${RENDERDOCCMD_EXECUTABLE}"
                    --render-system "${RENDER_SYSTEM}"
                    --frame-count 5
                    --reference-dir "${CMAKE_SOURCE_DIR}/src/test/graphics"
                WORKING_DIRECTORY "${_BINARY_DIR}"
            )
        ENDFOREACH()
    ELSE()
        MESSAGE("-- Skipping capture tests for target ${MGE_CAPTURE_TEST_TARGET}")
    ENDIF()
ENDFUNCTION()
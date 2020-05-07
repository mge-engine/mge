# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
OPTION(COVERAGE_TESTS "Run coverage tests to determine code coverage" ON)
OPTION(DYNAMORIO_PATH "Path to Dynamo Rio Tools" "")
IF(WIN32)
    IF(COVERAGE_TESTS)
        MESSAGE("-- Coverage tests are enabled")
        IF(DYNAMIOIO_PATH)
            FIND_PROGRAM(DRRUN
                NAMES drrun.exe
                PATH "${DYNAMORIO_PATH}/bin64"
            )
        ELSE()
            FIND_PROGRAM(DRRUN
                NAMES drrun.exe
            )
        ENDIF()
        FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/coverage")
        IF(DRRUN STREQUAL "DRRUN-NOTFOUND")
            MESSAGE("-- Coverage tests are disabled, dynamo rio tools not found")
        ELSE()
            GET_FILENAME_COMPONENT(DRRUN_PATH ${DRRUN} DIRECTORY)
            SET(DRCOV2LCOV "${DRRUN_PATH}/../tools/bin64/drcov2lcov.exe")
            SET(COVERAGE_TESTS_ENABLED ON)
        ENDIF()
    ELSE()
        MESSAGE("-- Coverage tests are disabled")
    ENDIF()
ELSE()
    MESSAGE("-- Unsupported operating system for coverage tests")
ENDIF()

ADD_CUSTOM_TARGET(coverage-tests-run)

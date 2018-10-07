# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
OPTION(COVERAGE_TESTS "Run coverage tests to determine code coverage" ON)
OPTION(DYNAMICRIO_PATH "Path to Dynamic Rio Tools" "")
IF(COVERAGE_TESTS)
    MESSAGE("-- Coverage tests are enabled")
    IF(DYNAMICRIO_PATH)
        FIND_PROGRAM(DRRUN
            NAMES drrun.exe
            PATH "${DYNAMICRIO_PATH}/bin64"
        )
    ELSE()
        FIND_PROGRAM(DRRUN
            NAMES drrun.exe
        )
    ENDIF()
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/coverage")
    IF(DRRUN STREQUAL "DRRUN-NOTFOUND")
        MESSAGE("-- Coverage tests are disabled, dynamic rio tools not found")
    ELSE()
        GET_FILENAME_COMPONENT(DRRUN_PATH ${DRRUN} DIRECTORY)
        SET(DRCOV2LCOV "${DRRUN_PATH}/../tools/bin64/drcov2lcov.exe")
    ENDIF()
ELSE()
    MESSAGE("-- Coverage tests are disabled")
ENDIF()

ADD_CUSTOM_TARGET(coverage-tests-run)

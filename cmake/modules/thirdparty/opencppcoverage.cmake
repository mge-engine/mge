# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)

IF(WIN32)
    INCLUDE(thirdparty/scoop)
    IF(SCOOP_EXECUTABLE)
        EXECUTE_PROCESS(COMMAND cmd /c "${SCOOP_EXECUTABLE}" prefix opencppcoverage
            OUTPUT_VARIABLE SCOOP_OPENCPPCOVERAGE_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)
        FIND_PROGRAM(OPENCPPCOVERAGE_EXECUTABLE OpenCppCoverage.exe
            HINTS "${SCOOP_OPENCPPCOVERAGE_PREFIX}")
    ELSE()
        FIND_PROGRAM(OPENCPPCOVERAGE_EXECUTABLE OpenCppCoverage.exe)
    ENDIF()

    IF(OPENCPPCOVERAGE_EXECUTABLE)
        MESSAGE(STATUS "OpenCppCoverage found at ${OPENCPPCOVERAGE_EXECUTABLE}")
    ELSE()
        MESSAGE(STATUS "OpenCppCoverage not found")
    ENDIF()
ENDIF()

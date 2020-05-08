# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

FUNCTION(TARGET_ADD_DEFINE TARGET DEFINE)
    GET_TARGET_PROPERTY(tmp ${TARGET} COMPILE_FLAGS)
    IF(tmp STREQUAL "tmp-NOTFOUND")
        SET(tmp "") # set to empty string
    ELSE()
        SET(tmp "${tmp} ") # a space to cleanly separate from existing content
    ENDIF()
    SET(tmp "${tmp} -D${DEFINE}")
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "${tmp}")
ENDFUNCTION()

FUNCTION(TARGET_ADD_INCLUDE TARGET DIRECTORY)
    SET_PROPERTY(TARGET ${TARGET} APPEND PROPERTY INCLUDE_DIRECTORIES ${DIRECTORY})
ENDFUNCTION()


FUNCTION(TARGET_ADD_LIBDIR TARGET DIRECTORY)
    GET_TARGET_PROPERTY(tmp ${TARGET} LINK_FLAGS)
    IF(tmp STREQUAL "tmp-NOTFOUND")
        SET(tmp "") # set to empty string
    ELSE()
        SET(tmp "${tmp} ") # a space to cleanly separate from existing content
    ENDIF()
    IF(MSVC)
        SET(tmp "${tmp} /LIBPATH:${DIRECTORY}")
    ELSE()
        SET(tmp "${tmp} -L${DIRECTORY}")
    ENDIF()
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "${tmp}")
ENDFUNCTION()

FUNCTION(COPY_FILE SOURCE TARGET)
    SET(__target_name "copy-${SOURCE}-to-${TARGET}")
    STRING(REPLACE "/" "_" __target_name "${__target_name}")
    STRING(REPLACE ":" "_" __target_name "${__target_name}")
    STRING(REPLACE "." "_" __target_name "${__target_name}")
    STRING(REPLACE " " "_" __target_name "${__target_name}")
    ADD_CUSTOM_TARGET("${__target_name}"
                      ALL
                      DEPENDS ${TARGET})
    ADD_CUSTOM_COMMAND(OUTPUT ${TARGET}
                       COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE} ${TARGET}
                       DEPENDS ${SOURCE})
ENDFUNCTION()

FUNCTION(TARGET_IS_TEST TARGET)

    SET(_FLAGS ${ARGV})
    LIST(POP_FRONT _FLAGS)
    IF(NEEDSDISPLAY IN_LIST _FLAGS)
        IF(HEADLESS_ENVIRONMENT)
            MESSAGE("-- Skipping ${TARGET} test due to headless environment")
            RETURN()
        ENDIF()
    ENDIF()


    GET_FILENAME_COMPONENT(_PLAIN_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM}" NAME)
    IF("${_PLAIN_MAKE_PROGRAM}" STREQUAL "MSBuild.exe")
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
    ELSE()
        SET(_BINARY_DIR "${CMAKE_BINARY_DIR}")
    ENDIF()
    ADD_TEST(NAME ${TARGET}
             COMMAND ${_BINARY_DIR}/${TARGET} --gtest_output=xml:${TARGET}.xml
             WORKING_DIRECTORY "${_BINARY_DIR}")
    IF(COVERAGE_TESTS_ENABLED)
        IF(NOCOVERAGE IN_LIST _FLAGS)
            MESSAGE("-- Not running test ${TARGET} as coverage test")
        ELSE()
            ADD_CUSTOM_TARGET("${TARGET}-coverage-test"
                COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_SOURCE_DIR}/tools/removelog.py" "${CMAKE_BINARY_DIR}/coverage/drcov.${TARGET}"
                COMMAND ${DRRUN} -t drcov -logdir ${CMAKE_BINARY_DIR}/coverage -- ${_BINARY_DIR}/${TARGET} --gtest_output=xml:${TARGET}-cov.xml
                WORKING_DIRECTORY "${_BINARY_DIR}"
            )
            ADD_DEPENDENCIES(coverage-tests ${TARGET}-coverage-test)
        ENDIF()
    ENDIF()
ENDFUNCTION()

FUNCTION(DUMP_ALL)
    MESSAGE(STATUS "==============================================================")
    MESSAGE(STATUS "CMAKE VARIABLES")
    MESSAGE(STATUS "==============================================================")
    GET_CMAKE_PROPERTY(_variableNames VARIABLES)
    FOREACH(_variableName ${_variableNames})
        MESSAGE(STATUS "${_variableName}=${${_variableName}}")
    ENDFOREACH()
    MESSAGE(STATUS "==============================================================")
ENDFUNCTION()


EXECUTE_PROCESS(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

LIST(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)
LIST(REMOVE_ITEM CMAKE_PROPERTY_LIST LOCATION)
LIST(REMOVE_ITEM CMAKE_PROPERTY_LIST "LOCATION_<CONFIG>")
LIST(REMOVE_ITEM CMAKE_PROPERTY_LIST "VS_DEPLOYMENT_LOCATION")

FUNCTION(PRINT_TARGET_PROPERTIES TGT)
    MESSAGE(STATUS "==============================================================")
    MESSAGE(STATUS "PROPERTIES OF TARGET ${TGT}")
    MESSAGE(STATUS "==============================================================")

    IF(NOT TARGET ${TGT})
        MESSAGE(STATUS "There is no target named '${TGT}'")
        MESSAGE(STATUS "==============================================================")
        RETURN()
    ENDIF()

    FOREACH (PROP ${CMAKE_PROPERTY_LIST})
        STRING(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" PROP ${PROP})
        GET_TARGET_PROPERTY(PROPVAL ${TGT} ${PROP})
        IF (PROPVAL)
            GET_TARGET_PROPERTY(PROPVAL ${TGT} ${PROP})
            MESSAGE(STATUS "  ${PROP} = ${PROPVAL}")
        ENDIF()
    ENDFOREACH(PROP)
    MESSAGE(STATUS "==============================================================")
ENDFUNCTION(PRINT_TARGET_PROPERTIES)



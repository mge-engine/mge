# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

FUNCTION(PRINT_VARIABLES)
    GET_CMAKE_PROPERTY(_variablenames VARIABLES)
    LIST(SORT _variablenames)

    MESSAGE(STATUS "==============================================================")
    MESSAGE(STATUS "ALL DEFINED VARIABLES")
    MESSAGE(STATUS "==============================================================")

    FOREACH(_var ${_variablenames})
        MESSAGE(STATUS "VARIABLE: ${_var}")
        MESSAGE(STATUS "VALUE   : ${${_var}}")
        MESSAGE(STATUS "==============================================================")
    ENDFOREACH()

ENDFUNCTION()

EXECUTE_PROCESS(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

LIST(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)

FUNCTION(PRINT_TARGET_PROPERTIES TGT)

    EXECUTE_PROCESS(COMMAND cmake --help-property-list OUTPUT_VARIABLE _CMAKE_PROPERTY_LIST)
    STRING(REGEX REPLACE ";" "\\\\;" _CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
    STRING(REGEX REPLACE "\n" ";" _CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

    LIST(REMOVE_DUPLICATES _CMAKE_PROPERTY_LIST)

    MESSAGE(STATUS "==============================================================")
    MESSAGE(STATUS "PROPERTIES OF TARGET ${TGT}")
    MESSAGE(STATUS "==============================================================")

    IF(NOT TARGET ${TGT})
        MESSAGE(STATUS "There is no target named '${TGT}'")
        MESSAGE(STATUS "==============================================================")
        RETURN()
    ENDIF()

    FOREACH (PROP ${_CMAKE_PROPERTY_LIST})
        STRING(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" PROP ${PROP})
        GET_TARGET_PROPERTY(PROPVAL ${TGT} ${PROP})
        IF (PROPVAL)
            GET_TARGET_PROPERTY(PROPVAL ${TGT} ${PROP})
            MESSAGE(STATUS "  ${PROP} = ${PROPVAL}")
        ELSE()
            # MESSAGE(STATUS "  ${PROP} NOT SET")
        ENDIF()
    ENDFOREACH(PROP)
    MESSAGE(STATUS "==============================================================")
ENDFUNCTION(PRINT_TARGET_PROPERTIES)


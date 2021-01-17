# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.



FUNCTION(MGE_LIBRARY)
    # -- argument parsing
    SET(OPTIONS "")
    SET(ONE_VALUE_ARGS TARGET TYPE)
    SET(MULTI_VALUE_ARGS SOURCES)
    CMAKE_PARSE_ARGUMENTS(MGE_LIBRARY "${OPTIONS}" "${ONE_VALUE_ARGS}"
        "${MULTI_VALUE_ARGS}" "${ARGN}")

    # -- checks
    IF(NOT DEFINED MGE_LIBRARY_TARGET)
        MESSAGE(FATAL_ERROR "TARGET missing in call to MGE_LIBRARY")
    ENDIF()
    IF(NOT DEFINED MGE_LIBRARY_TYPE)
        MESSAGE(FATAL_ERROR "TYPE missing in call to MGE_LIBRARY for target ${MGE_LIBRARY_TARGET}")
    ENDIF()
    IF(NOT DEFINED MGE_LIBRARY_SOURCES)
        MESSAGE(FATAL_ERROR "SOURCES missing in call to MGE_LIBRARY for target ${MGE_LIBRARY_TARGET}")
    ENDIF()
    ADD_LIBRARY("${MGE_LIBRARY_TARGET}"
                "${MGE_LIBRARY_TYPE}"
                "${MGE_LIBRARY_SOURCES}")
    STRING(TOUPPER "${MGE_LIBRARY_TARGET}" MGE_LIBRARY_TARGET_UC)
    GET_TARGET_PROPERTY(_TMP ${MGE_LIBRARY_TARGET} COMPILE_FLAGS)
    IF(_TMP STREQUAL "_TMP-NOTFOUND")
        SET(_TMP "") # set to empty string
    ELSE()
        SET(_TMP "${_TMP} ") # a space to cleanly separate from existing content
    ENDIF()
    SET(_TMP "${_TMP} -DBUILD_${MGE_LIBRARY_TARGET_UC}")
    SET_TARGET_PROPERTIES(${MGE_LIBRARY_TARGET} PROPERTIES COMPILE_FLAGS "${_TMP}")
ENDFUNCTION()
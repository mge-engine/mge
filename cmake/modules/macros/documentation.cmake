# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

FUNCTION(MGE_DOCUMENT_TARGET TGT)
    GET_PROPERTY(__all_sources GLOBAL PROPERTY ALL_PROJECT_SOURCES)
    GET_TARGET_PROPERTY(__target_sources ${TGT} SOURCES)
    GET_TARGET_PROPERTY(__target_source_dir ${TGT} SOURCE_DIR)

    # sources are relative to source dir, so need to change that
    FOREACH(__source ${__target_sources})
        IF(IS_ABSOLUTE ${__source})
            LIST(APPEND __all_sources ${__source})
        ELSE()
            SET(__source "${__target_source_dir}/${__source}")
            FILE(TO_NATIVE_PATH ${__source} __source)
            LIST(APPEND __all_sources ${__source})
        ENDIF()
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES __all_sources)
    SET_PROPERTY(GLOBAL PROPERTY ALL_PROJECT_SOURCES "${__all_sources}")
ENDFUNCTION()

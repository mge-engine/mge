# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
IF(WIN32)
    SET(helper_scripts
        enable_trace.cmd
        disable_trace.cmd)

    FOREACH(FILE ${helper_scripts})
        COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
                  "${CMAKE_BINARY_DIR}/${FILE}")
    ENDFOREACH()

    ENDIF()


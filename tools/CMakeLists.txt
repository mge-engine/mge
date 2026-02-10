# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(WIN32)
    SET(helper_scripts
        enable.cmd
        enable.ps1
        disable.cmd
        disable.ps1
    )

    FOREACH(FILE ${helper_scripts})
        COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
                  "${CMAKE_BINARY_DIR}/${FILE}")
    ENDFOREACH()

    IF(RENDERDOCCMD_EXECUTABLE)
        FILE(TO_NATIVE_PATH "${RENDERDOCCMD_EXECUTABLE}" RENDERDOCCMD)
        CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/rdoc.cmd.in"
                  "${CMAKE_BINARY_DIR}/rdoc.cmd" @ONLY)
    ENDIF()

ENDIF()


# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(WIN32)
    SET(helper_scripts
        enable_trace.cmd
        disable_trace.cmd
        enable_opengl.cmd
        enable_vulkan.cmd
        enable_dx11.cmd
        enable_dx12.cmd
        reset_render_system.cmd)

    FOREACH(FILE ${helper_scripts})
        COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
                  "${CMAKE_BINARY_DIR}/${FILE}")
    ENDFOREACH()

    ENDIF()


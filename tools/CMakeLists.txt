# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
IF(WIN32)
    SET(helper_scripts
        enable_trace.cmd
        enable_trace.ps1
        disable_trace.cmd
        disable_trace.ps1
        enable_opengl.cmd
        enable_opengl.ps1
        enable_vulkan.cmd
        enable_vulkan.ps1
        enable_dx11.cmd
        enable_dx11.ps1
        enable_dx12.cmd
        enable_dx12.ps1
        reset_render_system.cmd
        reset_render_system.ps1)

    FOREACH(FILE ${helper_scripts})
        COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
                  "${CMAKE_BINARY_DIR}/${FILE}")
    ENDFOREACH()

    IF(RENDERDOCCMD_EXECUTABLE)
        CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/run_rdoc.cmd.in"
                  "${CMAKE_BINARY_DIR}/run_rdoc.cmd")
    ENDIF()

ENDIF()


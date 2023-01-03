# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF(OpenGL_FOUND)
    ADD_SUBDIRECTORY(opengl)
ELSE()
    MESSAGE("-- OpenGL not found, not building opengl render system")
ENDIF()
IF(Vulkan_FOUND)
ADD_SUBDIRECTORY(vulkan)
ELSE()
    MESSAGE("-- Vulkan not found, not building vulkan render system")
ENDIF()

IF(MSVC)
    MESSAGE("-- Compiler is Visual C++, checking whether to enable DirectX support")
    IF(DirectX_D3D12_FOUND)
        MESSAGE("-- DirectX D3D12 found and enabled")
        ADD_SUBDIRECTORY(directx12)
    ELSE()
        MESSAGE("-- DirectX D3D12 not found")
    ENDIF()

    IF(DirectX_D3D11_FOUND)
        MESSAGE("-- DirectX D3D11 found and enabled")
        ADD_SUBDIRECTORY(directx11)
    ELSE()
        MESSAGE("-- DirectX D3D11 not found")
    ENDIF()
ENDIF()

ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(python)
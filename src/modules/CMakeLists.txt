# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_SUBDIRECTORY(opengl)
IF(MSVC)
    MESSAGE("-- Compiler is Visual C++, checking whether to enable DirectX support")
#    IF(DirectX_D3D12_FOUND)
#        ADD_SUBDIRECTORY(directx12)
#    ENDIF()
    
    IF(DirectX_D3D11_FOUND)
        MESSAGE("-- DirectX D3D11 found and enabled")
        ADD_SUBDIRECTORY(directx11)
    ELSE()
        MESSAGE("-- DirectX D3D11 not found")
    ENDIF()
ENDIF()
ADD_SUBDIRECTORY(vulkan)
ADD_SUBDIRECTORY(lua)

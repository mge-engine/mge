# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_SUBDIRECTORY(opengl)
IF(MSVC)
#    IF(DirectX_D3D12_FOUND)
#        ADD_SUBDIRECTORY(directx12)
#    ENDIF()
    IF(DirectX_D3D11_FOUND)
        ADD_SUBDIRECTORY(directx11)
   ENDIF()
ENDIF()
#ADD_SUBDIRECTORY(vulkan)
ADD_SUBDIRECTORY(lua)

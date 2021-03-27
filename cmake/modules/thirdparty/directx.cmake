# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(DirectX)

IF(DEFINED DirectX_DXGI_LIBRARY)
    ADD_LIBRARY(dxgi UNKNOWN IMPORTED)
    SET_PROPERTY(TARGET dxgi
                PROPERTY IMPORTED_LOCATION ${DirectX_DXGI_LIBRARY})
    SET_PROPERTY(TARGET dxgi
                APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DirectX_INCLUDE_DIR})
ENDIF()


IF(DEFINED DirectX_D3DCOMPILER_LIBRARY)
    ADD_LIBRARY(d3dcompiler UNKNOWN IMPORTED)
    SET_PROPERTY(TARGET d3dcompiler
                PROPERTY IMPORTED_LOCATION ${DirectX_D3DCOMPILER_LIBRARY})
    SET_PROPERTY(TARGET d3dcompiler
                APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DirectX_INCLUDE_DIR})
ENDIF()

IF(DirectX_D3D11_FOUND)
    ADD_LIBRARY(dx11 UNKNOWN IMPORTED)

    SET_PROPERTY(TARGET dx11
                PROPERTY IMPORTED_LOCATION ${DirectX_D3D11_LIBRARY})
    SET_PROPERTY(TARGET dx11
                APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DirectX_D3D11_INCLUDE_DIR})
    SET_PROPERTY(TARGET dx11
                PROPERTY INTERFACE_LINK_LIBRARIES dxgi d3dcompiler dxguid)
    MESSAGE("-- DirectX 11 Library: ${DirectX_D3D11_LIBRARY}")
ENDIF()

IF(DirectX_D3D12_FOUND)
    MESSAGE("-- DirextX 12 found")
    ADD_LIBRARY(dx12 UNKNOWN IMPORTED)
    SET_PROPERTY(TARGET dx12
                PROPERTY IMPORTED_LOCATION ${DirectX_D3D12_LIBRARY})
    SET_PROPERTY(TARGET dx12
                APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${DirectX_D3D12_INCLUDE_DIR})
    SET_PROPERTY(TARGET dx12
                PROPERTY INTERFACE_LINK_LIBRARIES dxgi d3dcompiler dxguid)
    MESSAGE("-- DirectX 12 Library: ${DirectX_D3D12_LIBRARY}")
ENDIF()

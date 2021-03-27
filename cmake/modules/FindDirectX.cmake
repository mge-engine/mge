# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
MESSAGE("-- Looking for DirectX SDK (as part of Windows Kit")


IF(WIN32)
  # get all installed kits and reverse the list to prefer the latest
  IF(CMAKE_CL_64)
    MESSAGE("--   Using x64 windows sdk libraries")
    SET(DirectX_LIBPATH_SUFFIX "x64")
  ELSE()
    MESSAGE("--   Using x86 windows sdk libraries")
    SET(DirectX_LIBPATH_SUFFIX "x86")
  ENDIF()
  FILE(GLOB __windows_kits_libs
       LIST_DIRECTORIES TRUE
       "C:/Program Files (x86)/Windows Kits/10/Lib/*/um/${DirectX_LIBPATH_SUFFIX}")
  LIST(REVERSE __windows_kits_libs)
  FILE(GLOB __windows_kits_includes
       LIST_DIRECTORIES TRUE
       "C:/Program Files (x86)/Windows Kits/10/Include/*/shared")
  FILE(GLOB __windows_kits_um_includes
       LIST_DIRECTORIES TRUE
       "C:/Program Files (x86)/Windows Kits/10/Include/*/um")
  SET(DirectX_INC_SEARCH_PATH
      "${__windows_kits_includes};${__windows_kits_um_includes}")
  LIST(SORT DirectX_INC_SEARCH_PATH)
  LIST(REVERSE DirectX_INC_SEARCH_PATH)
  SET(DirectX_LIB_SEARCH_PATH
      ${__windows_kits_libs})
  MESSAGE("--   DirectX_LIB_SEARCH_PATH ${DirectX_LIB_SEARCH_PATH}")
  MESSAGE("--   DirectX_INC_SEARCH_PATH ${DirectX_INC_SEARCH_PATH}")
  FIND_PATH(DirectX_INCLUDE_DIR NAMES d3d9.h HINTS ${DirectX_INC_SEARCH_PATH})
  MESSAGE("--   DirectX_INCLUDE_DIR set to ${DirectX_INCLUDE_DIR}")
  FIND_LIBRARY(DirectX_LIBRARY NAMES d3d9 HINTS ${DirectX_LIB_SEARCH_PATH})
  MESSAGE("--   DirectX_LIBRARY set to ${DirectX_LIBRARY}")
  FIND_LIBRARY(DirectX_D3DX9_LIBRARY NAMES d3dx9 HINTS ${DirectX_LIB_SEARCH_PATH})
  FIND_LIBRARY(DirectX_DXERR_LIBRARY NAMES DxErr HINTS ${DirectX_LIB_SEARCH_PATH})
  FIND_LIBRARY(DirectX_DXGUID_LIBRARY NAMES dxguid HINTS ${DirectX_LIB_SEARCH_PATH})

  FIND_LIBRARY(DirectX_DXGI_LIBRARY NAMES dxgi HINTS ${DirectX_LIB_SEARCH_PATH})
  MESSAGE("--   DirectX_DXGI_LIBRARY set to ${DirectX_DXGI_LIBRARY}")
  FIND_LIBRARY(DirectX_D3DCOMPILER_LIBRARY NAMES d3dcompiler HINTS ${DirectX_LIB_SEARCH_PATH})
  MESSAGE("--   DirectX_D3DCOMPILER_LIBRARY set to ${DirectX_D3DCOMPILER_LIBRARY}")

  FIND_PATH(DirectX_D3D11_INCLUDE_DIR NAMES d3d11.h HINTS ${DirectX_INC_SEARCH_PATH})
  MESSAGE("--   DirectX_D3D11_INCLUDE_DIR set to ${DirectX_D3D11_INCLUDE_DIR}")
  FIND_LIBRARY(DirectX_D3D11_LIBRARY NAMES d3d11 HINTS ${DirectX_LIB_SEARCH_PATH})
  MESSAGE("--   DirectX_D3D11_LIBRARY set to ${DirectX_D3D11_LIBRARY}")
  FIND_LIBRARY(DirectX_D3DX11_LIBRARY NAMES d3dx11 HINTS ${DirectX_LIB_SEARCH_PATH})
  IF(DirectX_D3D11_INCLUDE_DIR AND DirectX_D3D11_LIBRARY)
    SET(DirectX_D3D11_FOUND TRUE)
    MESSAGE("--   DirectX 11 SDK was found")
  ENDIF()

  FIND_PATH(DirectX_D3D12_INCLUDE_DIR NAMES d3d12.h HINTS ${DirectX_INC_SEARCH_PATH})
  MESSAGE("--   DirectX_D3D12_INCLUDE_DIR set to ${DirectX_D3D12_INCLUDE_DIR}")
  FIND_LIBRARY(DirectX_D3D12_LIBRARY NAMES D3D12 HINTS ${DirectX_LIB_SEARCH_PATH})
  MESSAGE("--   DirectX_D3D12_LIBRARY set to ${DirectX_D3D12_LIBRARY}")
  FIND_LIBRARY(DirectX_D3DX11_LIBRARY NAMES d3dx11 HINTS ${DirectX_LIB_SEARCH_PATH})
  IF(DirectX_D3D12_INCLUDE_DIR AND DirectX_D3D12_LIBRARY)
    SET(DirectX_D3D12_FOUND TRUE)
    MESSAGE("--   DirectX 12 SDK was found")
  ENDIF()
ENDIF()

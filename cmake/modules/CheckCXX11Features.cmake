# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
include(CheckCXXCompilerFlag)

function(check_cxx11_features)
    if (CMAKE_CXX_COMPILER MATCHES ".*clang")
        set(CMAKE_COMPILER_IS_CLANGXX 1)
    endif()

    if(${CMAKE_COMPILER_IS_GNUCXX})
        check_cxx_compiler_flag("-std=c++11" CXX_NEED_STD_CXX11)
        check_cxx_compiler_flag("-std=gnu++11" CXX_NEED_GNU_CXX11)
        if(${CXX_NEED_STD_CXX11})
            set(HAVE_CXX11 TRUE)
        endif()
        if(${CXX_NEED_GNU_CXX11})
            set(HAVE_CXX11 TRUE)
        endif()
    endif()

    if(${CMAKE_COMPILER_IS_CLANGXX})
        check_cxx_compiler_flag("-std=c++11" CXX_NEED_STD_CXX11)
        if(${CXX_NEED_STD_CXX11})
            set(HAVE_CXX11 TRUE)
        endif()
    endif()

    if(APPLE)
        set(HAVE_CXX11 TRUE)
        set(CXX_NEED_STD_CXX11 TRUE)
    endif()

    if(MSVC)
        if(NOT "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "18.0")
            set(HAVE_CXX11 TRUE)
        endif()
    endif()

    if(${CXX_NEED_GNU_CXX11})
        set(CXX_NEED_GNU_CXX11 "${CXX_NEED_GNU_CXX11}" PARENT_SCOPE)
        set(CXX_CXX11_FLAGS "-std=gnu++11" PARENT_SCOPE)
    else()
        if(${CXX_NEED_STD_CXX11})
            set(CXX_NEED_STD_CXX11 "${CXX_NEED_STD_CXX11}" PARENT_SCOPE)
            set(CXX_CXX11_FLAGS "-std=c++11" PARENT_SCOPE)
        endif()
    endif()

    if("${HAVE_CXX11}")
        set(HAVE_CXX11 "TRUE" PARENT_SCOPE)
    endif()
endfunction()

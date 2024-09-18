# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_ffi STATIC
    ffi.cpp)

TARGET_LINK_LIBRARIES(mge_ffi PRIVATE PkgConfig::LIBFFI)

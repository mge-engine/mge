# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mge_slang STATIC
    slang_compiler.cpp)

TARGET_LINK_LIBRARIES(mge_slang
    mgecore
    mgegraphics
    slang::slang)

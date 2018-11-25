# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_LIBRARY(mgereflection
            SHARED
            module.cpp
            type.cpp
            visitor.cpp
            signature.cpp
            function_signature.cpp
            constructor.cpp

            bind_builtin.cpp
            bind_std.cpp
            bind_core.cpp)


TARGET_ADD_DEFINE(mgereflection BUILD_MGE_REFLECTION)
TARGET_LINK_LIBRARIES(mgereflection mgecore)

ADD_SUBDIRECTORY(test)


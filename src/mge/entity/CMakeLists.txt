# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(mgeentity_sources
    dummy.cpp
)
ADD_LIBRARY(mgeentity
            SHARED
            ${mgeentity_sources})

TARGET_ADD_DEFINE(mgeentity BUILD_MGE_ENTITY)
TARGET_LINK_LIBRARIES(mgeentity mgecore EnTT)
ADD_SUBDIRECTORY(test)


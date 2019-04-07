# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mgeasset_sources
    asset_locator.cpp
)

ADD_LIBRARY(mgeasset
            SHARED
            ${mgeasset_sources})

TARGET_ADD_DEFINE(mgeasset BUILD_MGE_ASSET)
TARGET_LINK_LIBRARIES(mgeasset mgecore)

#ADD_SUBDIRECTORY(test)


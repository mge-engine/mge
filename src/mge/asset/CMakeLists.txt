# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mgeasset_sources
    asset.cpp
    asset_access.cpp
    asset_access_factory.cpp 
    file_asset_factory.cpp
    asset_not_found.cpp
    asset_type.cpp
    asset_loader.cpp
    asset_type_mapper.cpp)

ADD_LIBRARY(mgeasset
            SHARED
            ${mgeasset_sources})

TARGET_ADD_DEFINE(mgeasset BUILD_MGE_ASSET)
TARGET_LINK_LIBRARIES(mgeasset mgecore)

ADD_SUBDIRECTORY(test)


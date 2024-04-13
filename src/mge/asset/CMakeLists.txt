# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mgeasset_sources
    asset_type.cpp
    asset_not_found.cpp
    asset.cpp
    asset_access_factory.cpp
    asset_access.cpp
    asset_loader.cpp
    file_asset_access.cpp
    file_asset_access_factory.cpp
)

SET(mgeasset_headers
    dllexport.hpp
    asset_type.hpp
    asset_not_found.hpp
    asset.hpp
    asset_access_factory.hpp
    asset_access.hpp
    asset_loader.hpp
    file_asset_access.hpp
    file_asset_access_factory.hpp
)

MGE_LIBRARY(
    TARGET  mgeasset
    TYPE    SHARED
    SOURCES
    ${mgeasset_sources}
    ${mgeasset_headers}
)

TARGET_LINK_LIBRARIES(mgeasset mgecore boost-all)

ADD_SUBDIRECTORY(test)

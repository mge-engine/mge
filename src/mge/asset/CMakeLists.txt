# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mgeasset_sources
    asset_type.cpp
    asset_handle.cpp
    asset_not_found.cpp
    asset_locator.cpp
    asset.cpp
)

SET(mgeasset_headers
    dllexport.hpp
    asset_type.hpp
    asset_handle.hpp
    asset_not_found.hpp
    asset_locator.hpp
    asset.hpp
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

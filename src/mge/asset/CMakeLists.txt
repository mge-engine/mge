# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mgeasset_sources
    asset_type.cpp
    asset_not_found.cpp
    asset.cpp
    asset_source.cpp
    asset_access.cpp
    asset_loader.cpp
    file_asset_access.cpp
    file_asset_source.cpp
    application_octet_stream_loader.cpp
    asset_corrupted.cpp
)

SET(mgeasset_headers
    dllexport.hpp
    asset_type.hpp
    asset_not_found.hpp
    asset.hpp
    asset_source.hpp
    asset_access.hpp
    asset_loader.hpp
    file_asset_access.hpp
    file_asset_source.hpp
    asset_corrupted.hpp
)


MGE_LIBRARY(
    TARGET  mgeasset
    TYPE    SHARED
    SOURCES
    ${mgeasset_sources}
    ${mgeasset_headers}
)

TARGET_LINK_LIBRARIES(mgeasset PRIVATE
    mgecore

    unofficial::libmagic::libmagic
    boost-all
)

ADD_EXECUTABLE(mgeassettool
    mgeassettool.cpp)

TARGET_LINK_LIBRARIES(mgeassettool
    PRIVATE 
    mgeasset 
    mgecore 
    mgegraphics
)


ADD_SUBDIRECTORY(test)

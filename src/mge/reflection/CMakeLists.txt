# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgereflection_sources
    module.cpp

    module_details.cpp
)

SET(mgereflection_headers
    module.hpp

    module_details.hpp
)

MGE_LIBRARY(
    TARGET      mgereflection
    TYPE        SHARED
    SOURCES     
    ${mgereflection_sources}
    ${mgereflection_headers}
)

TARGET_LINK_LIBRARIES(
    mgereflection
    mgecore
    boost-all
)

ADD_SUBDIRECTORY(test)

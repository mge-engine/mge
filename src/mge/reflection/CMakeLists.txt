# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgereflection_sources
    module.cpp

    module_details.cpp
    type_details.cpp
    type_identifier.cpp
)

SET(mgereflection_headers
    module.hpp
    type.hpp

    module_details.hpp
    type_details.hpp
    type_identifier.hpp
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

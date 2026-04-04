# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.


SET(mgescript_sources
    script_engine.cpp
    script_context.cpp
)

SET(mgescript_headers
    script_engine.hpp
    script_context.hpp
)

MGE_LIBRARY(
    TARGET      mgescript
    TYPE        SHARED
    SOURCES     
    ${mgescript_sources}
    ${mgescript_headers}
)

TARGET_LINK_LIBRARIES(
    mgescript

    mgereflection
    mgecore
    boost-all
)

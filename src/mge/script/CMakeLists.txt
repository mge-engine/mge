# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(mgescript_sources
    script_engine.cpp
    script_context.cpp

    script_module.cpp
    script_type.cpp

    bind_pod.cpp
)

SET(mgescript_headers
    dllexport.hpp
    script_fwd.hpp
    script_context.hpp
    script_engine.hpp

    script_module.hpp
    script_type.hpp
)


MGE_LIBRARY(
    TARGET  mgescript
    TYPE    SHARED
    SOURCES
    ${mgescript_sources}
    ${mgescript_headers}
)
TARGET_LINK_LIBRARIES(mgescript mgecore boost-all)
#ADD_SUBDIRECTORY(test)

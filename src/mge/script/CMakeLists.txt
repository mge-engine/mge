# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(mgescript_sources
    script_engine.cpp
    script_context.cpp

    module.cpp
    type.cpp
    function.cpp
    variable.cpp
    visitor.cpp
    bind.cpp

    module_details.cpp
    type_details.cpp
    function_details.cpp
    variable_details.cpp
)

SET(mgescript_headers
    dllexport.hpp
    script_fwd.hpp
    script_context.hpp
    script_engine.hpp

    module.hpp
    type.hpp
    visitor.hpp

    module_details.hpp
    type_details.hpp
)


MGE_LIBRARY(
    TARGET  mgescript
    TYPE    SHARED
    SOURCES
    ${mgescript_sources}
    ${mgescript_headers}
)
TARGET_LINK_LIBRARIES(mgescript mgecore boost-all)
ADD_SUBDIRECTORY(test)

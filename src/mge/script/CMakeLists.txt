# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgescript_sources
    script_engine.cpp
    script_context.cpp
    script_binder.cpp

    module.cpp

    module_data.cpp
    type_data.cpp
    function_data.cpp
)

 SET(mgescript_headers
    dllexport.hpp

    script_fwd.hpp
    script_engine.hpp
    script_context.hpp
    script_binder.hpp

    module.hpp

    module_data.hpp
    type_data.hpp   
    function_data.hpp
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

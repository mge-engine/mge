# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

SET(mgereflection_sources
    module.cpp

    module_details.cpp
    type_details.cpp
    function_details.cpp
    type_identifier.cpp
    call_context.cpp
    
    reflector.cpp
    std_reflector.cpp
)

SET(mgereflection_headers
    module.hpp
    type.hpp
    function.hpp

    module_details.hpp
    type_details.hpp
    function_details.hpp
    type_identifier.hpp
    call_context.hpp
    signature.hpp
    
    reflector.hpp
    
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

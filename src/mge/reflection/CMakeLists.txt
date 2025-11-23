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

SET(mge_module_reflection_std_SOURCES
    std_reflector.cpp)
ADD_LIBRARY(mge_module_reflection_std MODULE ${mge_module_reflection_std_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_reflection_std mgereflection mgecore)

SET(mge_module_reflection_core_SOURCES
    core_reflector.cpp)
ADD_LIBRARY(mge_module_reflection_core MODULE ${mge_module_reflection_core_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_reflection_core mgereflection mgecore)

SET(mge_module_reflection_graphics_SOURCES
    graphics_reflector.cpp)
ADD_LIBRARY(mge_module_reflection_graphics MODULE ${mge_module_reflection_graphics_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_reflection_graphics mgereflection mgecore mgegraphics)

SET(mge_module_reflection_math_SOURCES
    math_reflector.cpp)
ADD_LIBRARY(mge_module_reflection_math MODULE ${mge_module_reflection_math_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_reflection_math mgereflection mgecore mgemath)


ADD_SUBDIRECTORY(test)

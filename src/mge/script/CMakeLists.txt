# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

SET(mgescript_sources
    script_engine.cpp
    script_context.cpp
    script_binder.cpp

    module.cpp
    type.cpp
    function.cpp
    # variable.cpp
    visitor.cpp
    call_context.cpp
    invoke_context.cpp
    signature.cpp

    module_details.cpp
    type_details.cpp
    function_details.cpp
    # variable_details.cpp
)

SET(mgescript_headers
    dllexport.hpp

    script_fwd.hpp
    script_context.hpp
    script_engine.hpp
    script_binder.hpp

    module.hpp
    type.hpp
    function.hpp
    visitor.hpp
    signature.hpp
    traits.hpp

    module_details.hpp
    type_details.hpp
    function_details.hpp
)


MGE_LIBRARY(
    TARGET  mgescript
    TYPE    SHARED
    SOURCES
    ${mgescript_sources}
    ${mgescript_headers}
)
TARGET_LINK_LIBRARIES(mgescript mgecore boost-all)

SET(mge_module_script_core_SOURCES
    bind_core.cpp)
ADD_LIBRARY(mge_module_script_core MODULE ${mge_module_script_core_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_core mgescript mgecore)

SET(mge_module_script_application_SOURCES
    bind_application.cpp)
ADD_LIBRARY(mge_module_script_application MODULE ${mge_module_script_application_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_application mgescript mgeapplication)

SET(mge_module_script_graphics_SOURCES
    bind_graphics.cpp)
ADD_LIBRARY(mge_module_script_graphics MODULE ${mge_module_script_graphics_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_graphics mgescript mgegraphics)

SET(mge_module_script_math_SOURCES
    bind_math.cpp)
ADD_LIBRARY(mge_module_script_math MODULE ${mge_module_script_math_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_math mgescript mgemath)

ADD_SUBDIRECTORY(test)

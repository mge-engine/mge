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

    bind_std.cpp
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

SET(mge_module_script_core_SOURCES
    bind_core.cpp)
ADD_LIBRARY(mge_module_script_core MODULE ${mge_module_script_core_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_core mgescript mgecore)

SET(mge_module_script_application_SOURCES
    bind_application.cpp)
ADD_LIBRARY(mge_module_script_application MODULE ${mge_module_script_application_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_application mgescript mgecore mgeapplication)

SET(mge_module_script_input_SOURCES
    bind_input.cpp)
ADD_LIBRARY(mge_module_script_input MODULE ${mge_module_script_input_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_input mgescript mgecore mgeinput)

SET(mge_module_script_math_SOURCES
    bind_math.cpp)
ADD_LIBRARY(mge_module_script_math MODULE ${mge_module_script_math_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_math mgescript mgecore mgemath)

SET(mge_module_script_graphics_SOURCES
    bind_graphics.cpp)
ADD_LIBRARY(mge_module_script_graphics MODULE ${mge_module_script_graphics_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_graphics mgescript mgecore mgegraphics)

ADD_SUBDIRECTORY(test)

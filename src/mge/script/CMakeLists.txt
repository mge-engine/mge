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

SET(mge_module_script_core_SOURCES
    bind_core.cpp)
ADD_LIBRARY(mge_module_script_core MODULE ${mge_module_script_core_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_core mgescript mgecore)


SET(mge_module_script_application_SOURCES
    bind_application.cpp)
ADD_LIBRARY(mge_module_script_application MODULE ${mge_module_script_application_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_script_application mgescript mgecore mgeapplication)


ADD_SUBDIRECTORY(test)

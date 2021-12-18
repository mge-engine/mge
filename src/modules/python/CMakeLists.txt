# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.
SET(mge_python_module_SOURCES
    python_engine.cpp
    python_context.cpp
    python_bind_helper.cpp
    python_call_context.cpp
    python_type.cpp
    python_error.cpp
    python_module.cpp)

ADD_LIBRARY(mge_module_python MODULE
            ${mge_python_module_SOURCES})

TARGET_LINK_LIBRARIES(mge_module_python
    mgecore
    mgescript
    Python3::Python
    psapi)

ADD_EXECUTABLE(mgepython mgepython.cpp)
TARGET_LINK_LIBRARIES(mgepython mgecore mgescript)

ADD_SUBDIRECTORY(test)
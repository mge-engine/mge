# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

MGE_LIBRARY(
    TARGET  mgegraphics
    TYPE    SHARED
    SOURCES
        render_system.cpp
        extent.cpp
        point.cpp
        window_options.cpp
        window.cpp
        monitor.cpp

        ${CMAKE_SOURCE_DIR}/include/mge/graphics/dllexport.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/render_system.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/extent.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/point.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/window_options.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/window.hpp
        ${CMAKE_SOURCE_DIR}/include/mge/graphics/monitor.hpp

)
TARGET_LINK_LIBRARIES(mgegraphics mgecore boost-all)
ADD_SUBDIRECTORY(test)

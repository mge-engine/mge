# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mge_image_load_module_SOURCES
    stb_image_loader.cpp)
ADD_LIBRARY(mge_module_image_load MODULE
            ${mge_image_load_module_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_image_load PRIVATE mgegraphics mgeasset mgecore)
ADD_SUBDIRECTORY(test)

# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_module_stb_image_SOURCES
    stb_image.cpp
)

ADD_LIBRARY(mge_module_stb_image MODULE ${mge_module_stb_image_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_stb_image mgecore mgeasset mgegraphics)
# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(mge_module_assimp_SOURCES
    assimp.cpp
)
ADD_LIBRARY(mge_module_assimp MODULE ${mge_module_assimp_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_assimp mgecore mgeasset mgegraphics assimp::assimp)
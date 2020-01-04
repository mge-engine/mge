# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(mge_module_zip MODULE
            zip.cpp)
TARGET_LINK_LIBRARIES(mge_module_zip mgecore libzip)
ADD_SUBDIRECTORY(test)

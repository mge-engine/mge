# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mge_jpeg_module_SOURCES
    jpeg_loader.cpp)
ADD_LIBRARY(mge_module_jpeg MODULE
            ${mge_jpeg_module_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_jpeg PRIVATE mgegraphics mgeasset mgecore)

target_link_libraries(mge_module_jpeg PRIVATE ${JPEG_LIBRARIES})
target_include_directories(mge_module_jpeg PRIVATE ${JPEG_INCLUDE_DIR})

ADD_SUBDIRECTORY(test)

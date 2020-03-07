# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(asset_files
    red.jpg)

FOREACH(FILE ${asset_files})
    COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
              "${CMAKE_CURRENT_BINARY_DIR}/${FILE}")
ENDFOREACH()


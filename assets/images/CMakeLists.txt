# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
SET(asset_files
    red.jpg
    sand.png
)

FOREACH(ASSET_FILE ${asset_files})
    MESSAGE(STATUS "Copying asset file ${ASSET_FILE} to ${CMAKE_CURRENT_BINARY_DIR}")
    CONFIGURE_FILE(${ASSET_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${ASSET_FILE} COPYONLY)
ENDFOREACH(ASSET_FILE)
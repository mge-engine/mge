# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

SET(archive_files
    archive.zip
    file_asset.txt
    file_asset.txt.properties)

FOREACH(FILE ${archive_files})
    COPY_FILE("${CMAKE_CURRENT_SOURCE_DIR}/${FILE}"
              "${CMAKE_CURRENT_BINARY_DIR}/${FILE}")
ENDFOREACH()

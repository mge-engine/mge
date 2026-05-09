# mge - Modern Game Engine
# Copyright (c) 2017-2026 by Alexander Schroeder
# All rights reserved.
#
# Called at build time to copy the Python installation to DST_DIR,
# excluding .pyc files.

FILE(GLOB_RECURSE _files RELATIVE "${SRC_DIR}" "${SRC_DIR}/*")
FOREACH(_file ${_files})
    GET_FILENAME_COMPONENT(_file_base ${_file} NAME)
    IF(NOT _file_base MATCHES ".*\\.pyc")
        SET(_src "${SRC_DIR}/${_file}")
        SET(_dst "${DST_DIR}/${_file}")
        GET_FILENAME_COMPONENT(_dst_dir ${_dst} DIRECTORY)
        FILE(MAKE_DIRECTORY "${_dst_dir}")
        IF(NOT EXISTS "${_dst}" OR "${_src}" IS_NEWER_THAN "${_dst}")
            FILE(COPY_FILE "${_src}" "${_dst}")
        ENDIF()
    ENDIF()
ENDFOREACH()

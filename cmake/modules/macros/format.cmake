# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

INCLUDE(thirdparty/python)
INCLUDE(thirdparty/git)
INCLUDE(thirdparty/clang-format)

ADD_CUSTOM_TARGET(format
                 COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/tools/git-cmake-format.py" "--cmake" "${GIT_EXECUTABLE}" "${CLANG_FORMAT_EXECUTABLE}"
                 WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
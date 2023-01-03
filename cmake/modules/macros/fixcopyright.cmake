# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

INCLUDE(thirdparty/python)

ADD_CUSTOM_TARGET(fix-copyright
                  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/tools/fixcopyright.py"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
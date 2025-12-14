# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

# Samples may write temp stuff to <build root>/temp
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/temp)

ADD_SUBDIRECTORY(blackscreen)
ADD_SUBDIRECTORY(triangle)
ADD_SUBDIRECTORY(teapot)

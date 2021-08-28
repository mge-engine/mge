# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

CONFIGURE_FILE (
        "version.hpp.in"
        "version.hpp"
)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(math)
ADD_SUBDIRECTORY(graphics)
ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(asset)
ADD_SUBDIRECTORY(input)
ADD_SUBDIRECTORY(win32)
# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

CONFIGURE_FILE (
        "version.hpp.in"
        "version.hpp"
)

CONFIGURE_FILE (
        "config.hpp.in"
        "config.hpp"
)

ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(application)
ADD_SUBDIRECTORY(input)
ADD_SUBDIRECTORY(math)
ADD_SUBDIRECTORY(graphics)
ADD_SUBDIRECTORY(scene)
ADD_SUBDIRECTORY(script)
ADD_SUBDIRECTORY(asset)
ADD_SUBDIRECTORY(entity)
ADD_SUBDIRECTORY(ui)

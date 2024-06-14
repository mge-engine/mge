# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

ADD_SUBDIRECTORY(mge)
# TODO: Add support for Apple
if (NOT APPLE)
    ADD_SUBDIRECTORY(modules)
    ADD_SUBDIRECTORY(test)
    ADD_SUBDIRECTORY(samples)
endif()

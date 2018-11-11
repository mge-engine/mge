# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

ADD_SUBDIRECTORY(external)
ADD_SUBDIRECTORY(mge)
ADD_SUBDIRECTORY(modules)
IF(WIN32)
    ADD_SUBDIRECTORY(win32)
ENDIF()

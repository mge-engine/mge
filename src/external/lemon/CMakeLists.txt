# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.


SET(lemon_sources
    lemon.c)

ADD_EXECUTABLE(lemon
               ${lemon_sources})
IF(MSVC)
    TARGET_COMPILE_OPTIONS(lemon PRIVATE "-wd4702")
ENDIF()
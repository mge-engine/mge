# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

IF("$ENV{APPVEYOR}" STREQUAL "True")
    MESSAGE("-- Building in appveyor infrastructure")
    SET(MGE_VERSION_BUILD $ENV{APPVEYOR_BUILD_NUMBER})
ELSE()
   SET(MGE_VERSION_BUILD "UNKNWON")
ENDIF()


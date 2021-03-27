# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

OPTION(HEADLESS_ENVIRONMENT "Build runs in a headless environment" OFF)

IF(HEADLESS_ENVIRONMENT)
    MESSAGE("-- Running in headless environment, skipping tests requiring a graphics device")
ELSE()
    MESSAGE("-- Graphics device tests enabled")
ENDIF()
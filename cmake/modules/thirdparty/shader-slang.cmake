# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
INCLUDE_GUARD(GLOBAL)
FIND_PACKAGE(slang CONFIG REQUIRED)
IF(slang_FOUND)
    MESSAGE(STATUS "slang found")
ENDIF()

#shader-slang provides CMake targets:
#[cmake] 
#[cmake]   # this is heuristically generated, and may not be correct
#[cmake]   find_package(slang CONFIG REQUIRED)
#[cmake]   # note: 1 additional targets are not displayed.
#[cmake]   target_link_libraries(main PRIVATE slang::gfx slang::slang slang::slang-llvm slang::slang-glslang)

# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(LUAJIT REQUIRED IMPORTED_TARGET luajit)
#target_link_libraries(main PRIVATE PkgConfig::LUAJIT)

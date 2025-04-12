# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(LIBFFI REQUIRED IMPORTED_TARGET libffi)
#target_link_libraries(main PRIVATE PkgConfig::LIBFFI)

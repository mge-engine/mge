# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(LIBZIP REQUIRED libzip)

ADD_LIBRARY(libzip UNKNOWN IMPORTED)
SET_PROPERTY(TARGET libzip
             PROPERTY IMPORTED_LOCATION "${LIBZIP_LINK_LIBRARIES}")
SET_PROPERTY(TARGET libzip
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LIBZIP_INCLUDE_DIRS}")

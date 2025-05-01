# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Boost 1.75)
ADD_LIBRARY(boost-all INTERFACE IMPORTED GLOBAL)
SET_PROPERTY(TARGET boost-all
              PROPERTY INTERFACE_LINK_LIBRARIES Boost::boost)


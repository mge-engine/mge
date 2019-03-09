# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Boost 1.69.0 REQUIRED COMPONENTS filesystem)
ADD_LIBRARY(boost-all INTERFACE IMPORTED GLOBAL)
SET_PROPERTY(TARGET boost-all
              PROPERTY INTERFACE_LINK_LIBRARIES Boost::boost Boost::dynamic_linking Boost::filesystem)


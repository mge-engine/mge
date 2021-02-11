# mge - Modern Game Engine
# Copyright (c) 2021 by Alexander Schroeder
# All rights reserved.

FIND_PACKAGE(Boost 1.75 COMPONENTS stacktrace_noop stacktrace_windbg)
ADD_LIBRARY(boost-all INTERFACE IMPORTED GLOBAL)
SET_PROPERTY(TARGET boost-all
              PROPERTY INTERFACE_LINK_LIBRARIES Boost::boost Boost::stacktrace_windbg)


# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
SET(mge_dx12_module_SOURCES
  common.cpp
  error.cpp
  window.cpp
  command_list.cpp
  render_context.cpp
  render_system.cpp
)
ADD_LIBRARY(mge_module_dx12 MODULE
    ${mge_dx12_module_SOURCES})
TARGET_LINK_LIBRARIES(mge_module_dx12 mgecore mgegraphics mge_win32 mgeapplication dx12)
ADD_SUBDIRECTORY(test)


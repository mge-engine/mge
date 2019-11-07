# mge - Modern Game Engine
# Copyright (c) 2018 by Alexander Schroeder
# All rights reserved.
ADD_LIBRARY(vkutils STATIC
            library.cpp
            instance.cpp)
TARGET_INCLUDE_DIRECTORIES(vkutils PRIVATE "${CMAKE_BINARY_DIR}/src/modules/vulkan/vk" "${CMAKE_BINARY_DIR}/src/modules/vulkan" "${CMAKE_SOURCE_DIR}/src/modules/vulkan")
TARGET_LINK_LIBRARIES(vkutils mgecore vulkan)


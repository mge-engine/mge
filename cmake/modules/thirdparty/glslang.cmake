# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

# mge - Modern Game Engine
# Copyright (c) 2017-2023 by Alexander Schroeder
# All rights reserved.

#INCLUDE(thirdparty/vulkan)
#IF(Vulkan_FOUND)
#    FIND_PACKAGE(glslang CONFIG REQUIRED)
#    IF(glslang_FOUND)
#        MESSAGE(STATUS "glslang found")
#        #ADD_LIBRARY(glslang ALIAS glslang::glslang)
#        #target_link_libraries(glslang PRIVATE glslang::glslang-default-resource-limits)
#target_link_libraries(main PRIVATE glslang::OSDependent glslang::glslang glslang::MachineIndependent glslang::GenericCodeGen glslang::glslang-default-resource-limits glslang::OGLCompiler glslang::SPVRemapper glslang::SPIRV glslang::HLSL)
#    ENDIF()
#ENDIF()

# glslang::glslang-default-resource-limits
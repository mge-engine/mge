// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/shared_library.hpp"
#include "vulkan.hpp"

namespace vulkan {

    class vulkan_library
    {
    public:
        vulkan_library();
        ~vulkan_library();

        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

    private:
        mge::shared_library_ref m_library;
    };

} // namespace vulkan
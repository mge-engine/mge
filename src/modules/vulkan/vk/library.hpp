// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once 
#include "mge/core/shared_library.hpp"
#include "mge/core/log.hpp"
#include "vulkan.hpp"
#include "vk_fwd.hpp"

namespace vk {

    /**
     * Vulkan library and entry point.
     */
    class library
    {
    public:
        library();
        ~library();
        static const library& instance();
        PFN_vkCreateInstance vkCreateInstance;
        PFN_vkDestroyInstance vkDestroyInstance;

    private:
        void resolve_create_instance();
        mge::shared_library_ref m_library;
    };
}


#define VK_LIB (vk::library::instance())

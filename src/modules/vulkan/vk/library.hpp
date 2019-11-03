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
        /**
         * Constructor. Loads vulkan library and initializes
         * functions as necessary to create and configure 
         * an instance.
         */
        library();
        ~library();

        /** 
         * Access loaded library.
         * @return loaded library
         */
        static const library& instance();
        
        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        
        PFN_vkCreateInstance vkCreateInstance;
        PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;

    private:
        template <typename FPTR>
        bool resolve_instance_function(VkInstance instance,
                                       FPTR& target,
                                       const char* name);


        void resolve_get_instance_proc();
        void resolve_functions();

        mge::shared_library_ref m_library;
    };
}


#define VK_LIB (vk::library::instance())

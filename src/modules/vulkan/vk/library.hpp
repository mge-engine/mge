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
        PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;

        const std::vector<const char*>& required_extensions() const;
        const std::vector<const char*>& layers() const;
        const std::vector<const char*>& extensions() const;
    private:
        template <typename FPTR>
        bool resolve_instance_function(VkInstance instance,
                                       FPTR& target,
                                       const char* name);


        void resolve_get_instance_proc();
        void resolve_functions();
        void process_extensions();
        void process_validation_layers();

        mge::shared_library_ref m_library;
        std::vector<VkExtensionProperties> m_extension_properties;
        std::vector<const char*> m_extensions;
        std::vector<const char*> m_required_extensions;
        std::vector<VkLayerProperties> m_layer_properties;
        std::vector<const char*> m_layers;
    };
}


#define VK_LIB (vk::library::instance())

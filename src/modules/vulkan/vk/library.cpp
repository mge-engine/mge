// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "library.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/singleton.hpp"

MGE_USE_LOG(VULKAN);

namespace vk {

    library::library()
    {
#ifdef MGE_OS_WINDOWS
        const char* vulkan_library_name = "vulkan-1.dll";
#else
        const char* vulkan_library_name = "libvulkan.so.1";
#endif
        MGE_DEBUG_LOG(VULKAN)<<"Loading vulkan library "<<vulkan_library_name;
        m_library = mge::shared_library::load(vulkan_library_name);
        resolve_get_instance_proc();
        resolve_functions();
        process_extensions();
    }

    library::~library()
    {}

    template <typename FPTR>
    static inline void resolve_function(mge::shared_library_ref& library,
                                        FPTR& target,
                                        const char* name)

    {
        void* ptr = library->symbol(name);
        target = (FPTR)ptr;
        if (target==nullptr) {
            MGE_WARNING_LOG(VULKAN)<<"Cannot resolve "<<name;
        } else {
            MGE_DEBUG_LOG(VULKAN)<<"Resolved "<<name;
        }
    }

    template <typename FPTR>
    bool library::resolve_instance_function(VkInstance instance,
                                            FPTR& target,
                                            const char* name)
    {
        if (vkGetInstanceProcAddr) {
            auto ptr = vkGetInstanceProcAddr(instance, name);
            if (!ptr) {
                MGE_WARNING_LOG(VULKAN)<<"Cannot resolve "<<name;
            } else {
                MGE_DEBUG_LOG(VULKAN)<<"Resolved "<<name;
            }
#pragma warning(push)
#pragma warning(disable : 4191)
            target = reinterpret_cast<FPTR>(ptr);
#pragma warning(pop)
            return target!=nullptr;
        } else {
            MGE_THROW(::vulkan::error)<<"Cannot resolve "<<name<<" : vkGetInstanceProcAddr not initialized ";
        }
    }

    void library::resolve_get_instance_proc()
    {
        resolve_function(m_library, this->vkGetInstanceProcAddr, "vkGetInstanceProcAddr");
        if (!this->vkGetInstanceProcAddr) {

            MGE_ERROR_LOG(VULKAN)<<"Could not resolve vkGetInstanceProcAddr function, Vulkan library is not usable";
            MGE_THROW(::vulkan::error)<<"vkGetInstanceProcAddr function cannot be resolved";
        }
    }

    void library::resolve_functions()
    {
#define RESOLVE_FUNCTION(NAME) resolve_instance_function(0, this->NAME, #NAME)

        RESOLVE_FUNCTION(vkCreateInstance);
        RESOLVE_FUNCTION(vkEnumerateInstanceExtensionProperties);

        if (!this->vkCreateInstance||!this->vkEnumerateInstanceExtensionProperties) {
            MGE_ERROR_LOG(VULKAN)<<"Could not resolve essential Vulkan function"<<std::endl;
            MGE_THROW(::vulkan::error)<<"Vulkan functions cannot be resolved";
        }
    }

    void library::process_extensions()
    {
        uint32_t count = 0;
        CHECK_VKRESULT(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr), vkEnumerateDeviceExtensionProperties);
        MGE_DEBUG_LOG(VULKAN)<<"Found "<<count<<" extensions";
        m_extension_properties.resize(count);
        CHECK_VKRESULT(vkEnumerateInstanceExtensionProperties(nullptr, &count, m_extension_properties.data()), 
                       vkEnumerateDeviceExtensionProperties);
        for (const auto& ep:m_extension_properties) {
            MGE_DEBUG_LOG(VULKAN)<<"Found extension: "<<ep.extensionName;
        }
            
    }

    size_t library::required_extensions_count() const
    {
        return 2;
    }

    const char** library::required_extensions() const
    {
        const char* extensions[] = {
            "VK_KHR_surface",
            "VK_KHR_win32_surface"
        };
        return extensions;
    }

    const library&
    library::instance()
    {
        static mge::singleton<library> s_instance;
        return *s_instance;
    }
}

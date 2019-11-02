// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "library.hpp"
#include "mge/core/log.hpp"
#include "mge/core/singleton.hpp"
#include "error.hpp"

MGE_USE_LOG(VULKAN);

namespace vk {
    
    library::library()
    {
#ifdef MGE_OS_WINDOWS
        const char *vulkan_library_name = "vulkan-1.dll";
#else
        const char *vulkan_library_name = "libvulkan.so.1";
#endif
        MGE_DEBUG_LOG(VULKAN) << "Loading vulkan library " << vulkan_library_name;
        m_library = mge::shared_library::load(vulkan_library_name);
        resolve_create_instance();
    }

    library::~library()
    {}

    template <typename FPTR>
    static inline void resolve_function(mge::shared_library_ref& library,
                                        FPTR& target,
                                        const char *name)
    {
        void *ptr = library->symbol(name);
        target = (FPTR)ptr;
        if (target == nullptr) {
            MGE_WARNING_LOG(VULKAN) << "Cannot resolve " << name;
        } else {
            MGE_DEBUG_LOG(VULKAN) << "Resolved " << name;
        }
    }


    void library::resolve_create_instance()
    {
        resolve_function(m_library, this->vkCreateInstance, "vkCreateInstance");
        resolve_function(m_library, this->vkDestroyInstance, "vkDestroyInstance");
        if (!this->vkCreateInstance) {
            MGE_ERROR_LOG(VULKAN) << "Could not resolve vkCreateInstance function, vulkan api broken!" << std::endl;
            MGE_THROW(::vulkan::error) << "vkCreateInstance function cannot be resolved";
        }
    }

    const library&
    library::instance()
    {
        static mge::singleton<library> s_instance;
        return *s_instance;

    }
}

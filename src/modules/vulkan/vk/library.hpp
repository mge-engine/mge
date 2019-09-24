#pragma once 
#include "mge/core/shared_library.hpp"
#include "mge/core/log.hpp"
#include "vulkan.hpp"
#include "vk_fwd.hpp"

namespace vk {

    class library 
    {
    public:
        library();
        ~library();
        static const library_ref& instance();
    };
}


#define VK_LIB (vk::library::instance())
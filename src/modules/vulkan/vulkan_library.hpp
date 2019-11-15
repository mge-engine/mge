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

}
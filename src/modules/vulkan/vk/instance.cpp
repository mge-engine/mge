#include "instance.hpp"
#include "library.hpp"
namespace vk {
    instance::instance()
        :m_vk_instance(0)
    {
        VkApplicationInfo app_info = {};
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName   = "";
        app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
        app_info.pEngineName        = "";
        app_info.engineVersion      = VK_MAKE_VERSION(1,0,0);

        VkInstanceCreateInfo create_info = {};
        create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo        = &app_info;
        create_info.enabledExtensionCount   = 0;
        create_info.ppEnabledExtensionNames = nullptr;
        create_info.enabledLayerCount       = 0;

        library::instance().vkCreateInstance(&create_info, nullptr /*allocation callback*/, &m_vk_instance);
    }

    instance::~instance()
    {
        if (m_vk_instance) {
            library::instance().vkDestroyInstance(m_vk_instance, nullptr /* allocation callback */);
        }
    }

}

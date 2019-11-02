#include "instance.hpp"
namespace vk {
    instance::instance()
    {
        VkApplicationInfo app_info = {};
        app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName   = "";
        app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
        app_info.pEngineName        = "";
        app_info.engineVersion      = VK_MAKE_VERSION(1,0,0);

    }
}

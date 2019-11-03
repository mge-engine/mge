#include "instance.hpp"
#include "library.hpp"
#include "error.hpp"

namespace vk {

instance::instance()
    : m_vk_instance(0)
{
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = nullptr;
    create_info.enabledLayerCount = 0;

    library::instance().vkCreateInstance(&create_info, nullptr /*allocation callback*/, &m_vk_instance);
    vkGetInstanceProcAddr = library::instance().vkGetInstanceProcAddr;
    if (m_vk_instance) {
        resolve_instance_functions();
    }
}

instance::~instance()
{
    if (m_vk_instance && vkDestroyInstance) {
        vkDestroyInstance(m_vk_instance, nullptr /* allocation callback */);
    }
}

template <typename PFN>
bool instance::resolve_function(PFN& fptr, const char* name)
{
    auto f = vkGetInstanceProcAddr(m_vk_instance, name);
    fptr = reinterpret_cast<PFN>(f);
    return fptr != nullptr;
}

void instance::resolve_instance_functions()
{
#define RESOLVE_FUNCTION(N)                                               \
    do {                                                                  \
        if (!resolve_function(this->N, #N)) {                             \
            MGE_THROW(vulkan::error) << "Cannot resolve function " << #N; \
        }                                                                 \
    } while (false)

    RESOLVE_FUNCTION(vkDestroyInstance);
}

}

#include "device.hpp"
#include "instance.hpp"
#include "error.hpp"

namespace vk {
    device::device(const instance_ref& instance)
        :vkGetDeviceProcAddr(instance->vkGetDeviceProcAddr)
    {
        float queue_priority = 1.0f;
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = instance->graphics_queue_family_index();
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;

        VkPhysicalDeviceFeatures device_features = {};

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = &queue_create_info;
        create_info.queueCreateInfoCount = 1;
        create_info.pEnabledFeatures = &device_features;
        
        create_info.enabledExtensionCount = 0;
        const auto& enabled_layers = instance->enabled_layers();
        if (!enabled_layers.empty()) {
            create_info.enabledLayerCount = static_cast<uint32_t>(enabled_layers.size());
            create_info.ppEnabledLayerNames = enabled_layers.data();
        }

        auto rc = instance->vkCreateDevice(instance->physical_device(),
                                           &create_info,
                                           nullptr,
                                           &m_vk_device);
        CHECK_VKRESULT(rc, vkCreateDevice);
        resolve_functions();
    }

    device::~device()
    {}

    template <typename T>
    T device::resolve(const char* name)
    {
        auto ptr = vkGetDeviceProcAddr(m_vk_device, name);
#ifdef MGE_COMPILER_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4191)
#endif
        return reinterpret_cast<T>(ptr);
#ifdef MGE_COMPILER_MSVC
#  pragma warning (pop)
#endif
    }

    void device::resolve_functions()
    {
#define RESOLVE(N) do { this->N = resolve<PFN_##N>(#N); if(!N) MGE_THROW(vulkan::error) << "Cannot resolve function " << #N; } while(false)
        RESOLVE(vkDestroyDevice);
    }

}
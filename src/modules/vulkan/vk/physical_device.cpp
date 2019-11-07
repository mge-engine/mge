#include "physical_device.hpp"

namespace vk {
    physical_device::physical_device(VkPhysicalDevice d)
        :m_device(d)
    {}

    physical_device::physical_device(const physical_device& pd)
        : m_device(pd.m_device)
    {}
}
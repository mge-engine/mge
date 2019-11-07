#include "instance.hpp"
#include "library.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"

#ifdef max
#  undef max
#endif
#include <limits>

MGE_USE_LOG(VULKAN);

namespace vk {

    instance::instance(const vulkan::system_config& config)
        :m_vk_instance(0)
        , m_physical_device_index(std::numeric_limits<size_t>::max())
        , m_debug(config.debug())
        , m_debug_utils_found(false)
    {
        m_physical_device_features = {};
        m_physical_device_properties = {};

        auto required_extensions = library::instance().required_extensions();
        compute_extensions(required_extensions, config);
        compute_enabled_layers(config);

        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = "";
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
        create_info.ppEnabledExtensionNames = required_extensions.data();

        create_info.enabledLayerCount = static_cast<uint32_t>(m_enabled_layers.size());
        create_info.ppEnabledLayerNames = m_enabled_layers.data();

        library::instance().vkCreateInstance(&create_info, nullptr /*allocation callback*/, &m_vk_instance);
        vkGetInstanceProcAddr = library::instance().vkGetInstanceProcAddr;
        if (m_vk_instance) {
            resolve_instance_functions();
        }

        if (m_debug_utils_found) {
            setup_debug_callback();
        }
        fetch_physical_devices();
        select_physical_device();
    }

    instance::~instance()
    {
        if (m_debug_utils_found && vkDestroyDebugUtilsMessengerEXT && m_vk_debug_utils_messenger) {
            vkDestroyDebugUtilsMessengerEXT(m_vk_instance,
                                            m_vk_debug_utils_messenger,
                                            nullptr /* allocation callback */);
        }

        if (m_vk_instance && vkDestroyInstance) {
            vkDestroyInstance(m_vk_instance, nullptr /* allocation callback */);
        }
    }

    template <typename PFN>
    bool instance::resolve_function(PFN& fptr, const char* name)
    {
        auto f = vkGetInstanceProcAddr(m_vk_instance, name);
#pragma warning(push)
#pragma warning(disable : 4191)
        fptr = reinterpret_cast<PFN>(f);
#pragma warning (pop)
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
        RESOLVE_FUNCTION(vkGetDeviceProcAddr);
        RESOLVE_FUNCTION(vkEnumeratePhysicalDevices);
        RESOLVE_FUNCTION(vkGetPhysicalDeviceProperties);
        RESOLVE_FUNCTION(vkGetPhysicalDeviceFeatures);
        RESOLVE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);

        if (m_debug_utils_found) {
            RESOLVE_FUNCTION(vkCreateDebugUtilsMessengerEXT);
            RESOLVE_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
        }
    }

    void instance::compute_extensions(std::vector<const char*>& extensions, const vulkan::system_config& config)
    {
        if (config.debug()) {
            for (const auto& e : library::instance().extensions()) {
                if (strcmp(e, "VK_EXT_debug_utils") == 0) {
                    extensions.push_back(e);
                    m_debug_utils_found = true;
                }
            }
        }
    }

    void instance::compute_enabled_layers(const vulkan::system_config& config)
    {
        const auto& layers = library::instance().layers();
        for (const auto& l : layers) {
            if (config.debug() && strcmp(l, "VK_LAYER_KHRONOS_validation") == 0) {
                m_enabled_layers.push_back(l);
            } else {
                for (const auto& s : config.layers()) {
                    if (s == l) {
                        m_enabled_layers.push_back(l);
                    }
                }
            }
        }

        for (const auto& l : m_enabled_layers) {
            MGE_DEBUG_LOG(VULKAN) << "Enabled layer: " << l;
        }
    }

    namespace {
        static std::ostream& operator <<(std::ostream& os, const VkDebugUtilsMessageSeverityFlagBitsEXT s)
        {
            bool first = true;
            os << "(";
            if (s & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
                os << "V";
                first = false;
            }
            if (s & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
                if (first) {
                    os << "I";
                    first = false;
                } else {
                    os << ", I";
                }
            }
            if (s & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
                if (first) {
                    os << "W"; 
                    first = false;
                } else {
                    os << ", W";
                }
            }
            if (s & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
                if (first) {
                    os << "E";
                } else {
                    os << ", E";
                }
            }
            os << ")";
            return os;
        }

        static std::ostream& operator <<(std::ostream& os, const VkDebugUtilsMessageTypeFlagBitsEXT& t)
        {
            bool first = true;
            os << "<";
            if (t & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
                os << "G"; 
                first = false;
            }
            if (t & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
                if (first) {
                    os << "V";
                    first = false;
                } else {
                    os << ", V";
                }
            }
            if (t & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
                if (first) {
                    os << "P";
                } else {
                    os << ", P";
                }
            }
            return os << ">";
        }
    }

    static VkBool32 VKAPI_PTR debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                             VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                             const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                             void* pUserData)
    {
        MGE_DEBUG_LOG(VULKAN) 
            << messageSeverity << " "
            << (VkDebugUtilsMessageTypeFlagBitsEXT)messageTypes << ": "
            << pCallbackData->pMessage;

        return VK_FALSE;
    }


    void instance::setup_debug_callback()
    {
        MGE_DEBUG_LOG(VULKAN) << "Install debug callback";
        VkDebugUtilsMessengerCreateInfoEXT create_info = {};
        
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = &debug_callback;
        create_info.pUserData = nullptr;
        CHECK_VKRESULT(vkCreateDebugUtilsMessengerEXT(m_vk_instance, &create_info, nullptr, &m_vk_debug_utils_messenger),
                       vkCreateDebugUtilsMessengerEXT);
    }

    void instance::fetch_physical_devices()
    {
        MGE_DEBUG_LOG(VULKAN) << "Fetching physical devices";
        uint32_t count = 0;
        CHECK_VKRESULT(vkEnumeratePhysicalDevices(m_vk_instance, &count, nullptr), vkEnumeratePhysicalDevices);
        if (count == 0) {
            MGE_THROW(vulkan::error) << "Found 0 physical devices, cannot initialize Vulkan";
        }
        m_physical_devices.resize(count);
        CHECK_VKRESULT(vkEnumeratePhysicalDevices(m_vk_instance, &count, m_physical_devices.data()), vkEnumeratePhysicalDevices);
        MGE_DEBUG_LOG(VULKAN) << "Found " << count << " physical devices";
    }

    void instance::select_physical_device()
    {
        for (size_t i = 0; i < m_physical_devices.size(); ++i) {
            if (physical_device_suitable(i)) {
                m_physical_device_index = i;
                break;
            }
        }

        if (m_physical_device_index > m_physical_devices.size()) {
            MGE_THROW(vulkan::error) << "No suitable physical device found";
        }
    }

    bool instance::physical_device_suitable(size_t index) 
    {
        VkPhysicalDeviceFeatures features = {};
        VkPhysicalDeviceProperties properties = {};

        vkGetPhysicalDeviceFeatures(m_physical_devices[index], &features);
        vkGetPhysicalDeviceProperties(m_physical_devices[index], &properties);


        MGE_DEBUG_LOG(VULKAN) << "Physical device: " << properties.deviceName;
        m_physical_device_features   = features;
        m_physical_device_properties = properties;
        return true;
    }



}

// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/log.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/executable_name.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif
#include "window.hpp"
#include "error.hpp"

MGE_USE_LOG(VULKAN);

namespace vulkan {

    render_system::render_system()
        :m_instance(VK_NULL_HANDLE)
        , m_debug_messenger(VK_NULL_HANDLE)
        , m_device(VK_NULL_HANDLE)
        , m_graphics_queue(VK_NULL_HANDLE)
        , m_compute_queue(VK_NULL_HANDLE)
        , m_transfer_queue(VK_NULL_HANDLE)
    {
        MGE_DEBUG_LOG(VULKAN) << "Creating Vulkan render system";
        m_library = std::make_shared<vulkan_library>();
    }


    render_system::~render_system()
    {
        if (m_device && vkDestroyDevice) {
            vkDestroyDevice(m_device, nullptr);
        }

        if (m_instance && m_debug_messenger && vkDestroyDebugUtilsMessengerEXT) {
            vkDestroyDebugUtilsMessengerEXT(m_instance, 
                                            m_debug_messenger, 
                                            nullptr);
        }

        if (m_instance && vkDestroyInstance) {
            vkDestroyInstance(m_instance, nullptr);
        }
    }


    void render_system::configure(const mge::configuration& config)
    {
        if (m_config.configured()) {
            MGE_THROW(mge::illegal_state) << "Render system is already configured";
        }
        m_config.configure(config);

        // for presenting to a window
        m_enabled_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        resolve_basic_instance_functions();
        scan_properties();
        init_instance_extensions();
        create_instance();
        resolve_normal_instance_functions();
        init_debug_message_handling();
        load_physical_devices();
        compute_queue_family_indices();
        create_device();
        resolve_device_functions();
        retrieve_device_queues();
    }

    render_system::monitor_collection_t render_system::monitors() const
    {
        return ::platform::monitor::all_monitors();
    }

    mge::monitor_ref render_system::primary_monitor() const
    {
        return platform::monitor::primary_monitor();
    }

    mge::window_ref render_system::create_window(const mge::rectangle& rect,
                                                 const mge::window_options& options) 
    {
        check_configure();
        return std::make_shared<window>(*this, rect, options, m_config);
    }

    void render_system::check_configure()
    {
        if (!m_config.configured()) {
            configure(mge::configuration("render.system"));
        }
    }

    void render_system::init_instance_extensions()
    {
        m_instance_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#ifdef MGE_OS_WINDOWS
        m_instance_extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
        if (m_config.validation() || m_config.debug()) {
            m_instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        for (const auto& name : m_instance_extensions) {
            bool found = false;
            for (const auto& e : m_all_instance_extensions) {
                if (strcmp(e.extensionName, name) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                MGE_THROW(vulkan::error) << "Required extension " << name << " not found";
            }
        }

    }

    void render_system::create_instance()
    {
        auto exe_name = mge::executable_name();

        VkApplicationInfo app_info = {};
        app_info.pApplicationName = exe_name.c_str();
        app_info.pEngineName = "mge";
        app_info.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount = static_cast<uint32_t>(m_instance_extensions.size());
        create_info.ppEnabledExtensionNames = m_instance_extensions.data();
        const char* enabled_layers[] = {
            "VK_LAYER_KHRONOS_validation"
        };
        if (m_config.validation()) {
            create_info.enabledLayerCount = 1;
            create_info.ppEnabledLayerNames = enabled_layers;
        }

        CHECK_VK_CALL(vkCreateInstance(&create_info, nullptr, &m_instance));
        
    }

    void render_system::resolve_basic_instance_functions()
    {
#ifdef MGE_COMPILER_MSVC
#  pragma warning (push)
#  pragma warning (disable: 4191)
#endif
#define RESOLVE(X)                                                                \
        do {                                                                      \
            auto f = m_library->vkGetInstanceProcAddr(VK_NULL_HANDLE, #X);        \
            if (!f) {                                                             \
                MGE_THROW(vulkan::error) << "Cannot resolve instance function: "  \
                    << #X;                                                        \
            }                                                                     \
            MGE_DEBUG_LOG(VULKAN) << "Resolve " << #X << ": " << (void *)f;       \
            this->X = reinterpret_cast<decltype(this->X)>(f);                     \
        }  while (false);

#define BASIC_INSTANCE_FUNCTION(X)  RESOLVE(X)
#define INSTANCE_FUNCTION(X)
#define DEVICE_FUNCTION(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#  include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#  pragma warning (pop)
#endif
    }

    void render_system::resolve_normal_instance_functions()
    {
#ifdef MGE_COMPILER_MSVC
#  pragma warning (push)
#  pragma warning (disable: 4191)
#endif
#define RESOLVE(X)                                                              \
        do {                                                                    \
            auto f = m_library->vkGetInstanceProcAddr(m_instance, #X);       \
            if (!f) {                                                           \
                MGE_WARNING_LOG(VULKAN) << "Cannot resolve " << #X;             \
            } else {                                                            \
                MGE_DEBUG_LOG(VULKAN) << "Resolve " << #X << ": " << (void *)f; \
                this->X = reinterpret_cast<decltype(this->X)>(f);               \
            }                                                                   \
        }  while (false);

#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
#define DEVICE_FUNCTION(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#  include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#  pragma warning (pop)
#endif

    }

    template <typename F, typename C>
    void render_system::fill_enumeration(const F& function, C& container)
    {
        uint32_t count = 0;
        function(&count, nullptr);
        if (count) {
            container.resize(count);
            function(&count, container.data());
        }
    }

    void render_system::scan_properties()
    {
        fill_enumeration
        ([&](uint32_t* count, VkExtensionProperties* data) {
            CHECK_VK_CALL(vkEnumerateInstanceExtensionProperties(nullptr, count, data));
         }, m_all_instance_extensions);

        for (const auto& e : m_all_instance_extensions) {
            MGE_DEBUG_LOG(VULKAN) << "Found instance extension: " << e.extensionName;
        }

        fill_enumeration
        ([&](uint32_t* count, VkLayerProperties* data) {
            CHECK_VK_CALL(vkEnumerateInstanceLayerProperties(count, data));
         }, m_all_instance_layers);

        for (const auto& l : m_all_instance_layers) {
            MGE_DEBUG_LOG(VULKAN) << "Found instance layer: " << l.layerName << "(" << l.description << ")";
        }

    }

    VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                        VkDebugUtilsMessageTypeFlagsEXT type,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                        void* user_data)
    {
        if (severity && VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            MGE_DEBUG_LOG(VULKAN) << "[" << callback_data->messageIdNumber << "][" << callback_data->pMessageIdName << "] : " << callback_data->pMessage;
        } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            MGE_INFO_LOG(VULKAN) << "[" << callback_data->messageIdNumber << "][" << callback_data->pMessageIdName << "] : " << callback_data->pMessage;
        } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            MGE_WARNING_LOG(VULKAN) << "[" << callback_data->messageIdNumber << "][" << callback_data->pMessageIdName << "] : " << callback_data->pMessage;
        } else {
            MGE_ERROR_LOG(VULKAN) << "[" << callback_data->messageIdNumber << "][" << callback_data->pMessageIdName << "] : " << callback_data->pMessage;
        }

        return VK_FALSE;
    }
    
    void render_system::init_debug_message_handling()
    {
        if (!m_config.debug() && !m_config.validation()) {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
        create_info.pfnUserCallback = &debug_utils_callback;
        CHECK_VK_CALL(vkCreateDebugUtilsMessengerEXT(m_instance,
                                                     &create_info,
                                                     nullptr,
                                                     &m_debug_messenger));
    }

    void render_system::load_physical_devices()
    {
        fill_enumeration([&](uint32_t* count, VkPhysicalDevice* data) {
                             CHECK_VK_CALL(vkEnumeratePhysicalDevices(m_instance, count, data));
                         }, m_physical_devices);
        if (m_physical_devices.empty()) {
            MGE_THROW(vulkan::error) << "No physical devices found";
        }
        m_selected_physical_device_index = 0;
        m_physical_device = m_physical_devices[m_selected_physical_device_index];

        vkGetPhysicalDeviceProperties(m_physical_device, &m_physical_device_properties);
        vkGetPhysicalDeviceFeatures(m_physical_device, &m_physical_device_features);
        vkGetPhysicalDeviceMemoryProperties(m_physical_device, &m_physical_device_memory_properties);

        fill_enumeration([&](uint32_t* count, VkQueueFamilyProperties* data) {
            vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, count, data);
                         }, m_physical_device_queue_family_properties);

        fill_enumeration
        ([&](uint32_t* count, VkExtensionProperties* data) {
            CHECK_VK_CALL(vkEnumerateDeviceExtensionProperties(m_physical_device, nullptr, count, data));
         }, m_physical_device_extensions);

        MGE_DEBUG_LOG(VULKAN) << "Selected physical device: " << m_physical_device_properties.deviceName;
    }

    uint32_t render_system::best_queue_family_index(VkQueueFlagBits flags)
    {
        if (flags & VK_QUEUE_COMPUTE_BIT) {
            for (uint32_t i = 0; i < m_physical_device_queue_family_properties.size(); ++i) {
                const auto& p = m_physical_device_queue_family_properties[i];
                if ((p.queueFlags & flags) && ((p.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)) {
                    return i;
                }
            }
        }

        if (flags & VK_QUEUE_TRANSFER_BIT) {
            for (uint32_t i = 0; i < m_physical_device_queue_family_properties.size(); ++i) {
                const auto& p = m_physical_device_queue_family_properties[i];
                if ((p.queueFlags & flags) 
                    && ((p.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) 
                    && ((p.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0) ) {
                    return i;
                }
            }
        }
        for (uint32_t i = 0; i < m_physical_device_queue_family_properties.size(); ++i) {
            const auto& p = m_physical_device_queue_family_properties[i];
            if (p.queueFlags & flags)    {
                return i;
            }
        }

        return std::numeric_limits<uint32_t>::max();
    }


    void render_system::compute_queue_family_indices()
    {
        m_queue_family_indices.compute = best_queue_family_index(VK_QUEUE_COMPUTE_BIT);
        m_queue_family_indices.transfer = best_queue_family_index(VK_QUEUE_TRANSFER_BIT);
        m_queue_family_indices.graphics = best_queue_family_index(VK_QUEUE_GRAPHICS_BIT);

        MGE_DEBUG_LOG(VULKAN) << "Compute queue family index: " << m_queue_family_indices.compute;
        MGE_DEBUG_LOG(VULKAN) << "Transfer queue family index: " << m_queue_family_indices.transfer;
        MGE_DEBUG_LOG(VULKAN) << "Graphics queue family index: " << m_queue_family_indices.graphics;

    }

    void render_system::create_device()
    {
        mge::small_vector<VkDeviceQueueCreateInfo, 3> queue_create_infos;
        
        const float default_queue_priority = 0.0f;

        VkDeviceQueueCreateInfo graphics_queue_create_info = {};
        graphics_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        graphics_queue_create_info.queueFamilyIndex = m_queue_family_indices.graphics;
        graphics_queue_create_info.queueCount = 1;
        graphics_queue_create_info.pQueuePriorities = &default_queue_priority;
        queue_create_infos.push_back(graphics_queue_create_info);

        VkDeviceQueueCreateInfo compute_queue_create_info = {};
        compute_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        compute_queue_create_info.queueFamilyIndex = m_queue_family_indices.compute;
        compute_queue_create_info.queueCount = 1;
        compute_queue_create_info.pQueuePriorities = &default_queue_priority;
        queue_create_infos.push_back(compute_queue_create_info);

        VkDeviceQueueCreateInfo transfer_queue_create_info = {};
        transfer_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        transfer_queue_create_info.queueFamilyIndex = m_queue_family_indices.transfer;
        transfer_queue_create_info.queueCount = 1;
        transfer_queue_create_info.pQueuePriorities = &default_queue_priority;
        queue_create_infos.push_back(transfer_queue_create_info);

        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.ppEnabledExtensionNames = m_enabled_device_extensions.data();
        create_info.enabledExtensionCount = static_cast<uint32_t>(m_enabled_device_extensions.size());

        CHECK_VK_CALL(vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device));
        MGE_DEBUG_LOG(VULKAN) << "Logical device created";
    }

    void render_system::resolve_device_functions()
    {
#ifdef MGE_COMPILER_MSVC
#  pragma warning (push)
#  pragma warning (disable: 4191)
#endif
#define RESOLVE(X)                                                                \
        do {                                                                      \
            auto f = vkGetDeviceProcAddr(m_device, #X);                           \
            if (!f) {                                                             \
                MGE_WARNING_LOG(VULKAN) << "Cannot resolve device function: "     \
                    << #X;                                                        \
            } else {                                                              \
                 MGE_DEBUG_LOG(VULKAN) << "Resolve " << #X << ": " << (void*)f;   \
            }                                                                     \
            this->X = reinterpret_cast<decltype(this->X)>(f);                     \
        }  while (false);

#define BASIC_INSTANCE_FUNCTION(X)  
#define INSTANCE_FUNCTION(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#  include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#  pragma warning (pop)
#endif

    }

    void render_system::retrieve_device_queues()
    {
        vkGetDeviceQueue(m_device, m_queue_family_indices.graphics, 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, m_queue_family_indices.compute, 0, &m_compute_queue);
        vkGetDeviceQueue(m_device, m_queue_family_indices.transfer, 0, &m_transfer_queue);
    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                vulkan);

}

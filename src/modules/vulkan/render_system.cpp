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
        :m_vk_instance(VK_NULL_HANDLE)
        ,m_vk_debug_messenger(VK_NULL_HANDLE)
    {
        MGE_DEBUG_LOG(VULKAN) << "Creating Vulkan render system";
        m_library = std::make_shared<vulkan_library>();
    }


    render_system::~render_system()
    {
        if (m_vk_instance && m_vk_debug_messenger && vkDestroyDebugUtilsMessengerEXT) {
            vkDestroyDebugUtilsMessengerEXT(m_vk_instance, 
                                            m_vk_debug_messenger, 
                                            nullptr);
        }

        if (m_vk_instance && vkDestroyInstance) {
            vkDestroyInstance(m_vk_instance, nullptr);
        }
    }


    void render_system::configure(const mge::configuration& config)
    {
        if (m_config.configured()) {
            MGE_THROW(mge::illegal_state) << "Render system is already configured";
        }
        m_config.configure(config);
        
        resolve_basic_instance_functions();
        scan_properties();
        init_instance_extensions();
        create_instance();
        resolve_normal_instance_functions();
        init_debug_message_handling();
        load_physical_devices();
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

        CHECK_VK_CALL(vkCreateInstance(&create_info, nullptr, &m_vk_instance));
        
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
            auto f = m_library->vkGetInstanceProcAddr(m_vk_instance, #X);       \
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
        CHECK_VK_CALL(vkCreateDebugUtilsMessengerEXT(m_vk_instance,
                                                     &create_info,
                                                     nullptr,
                                                     &m_vk_debug_messenger));
    }

    void render_system::load_physical_devices()
    {
        fill_enumeration([&](uint32_t* count, VkPhysicalDevice* data) {
                             CHECK_VK_CALL(vkEnumeratePhysicalDevices(m_vk_instance, count, data));
                         }, m_physical_devices);
        if (m_physical_devices.empty()) {
            MGE_THROW(vulkan::error) << "No physical devices found";
        }
        m_selected_physical_device = 0;

        vkGetPhysicalDeviceProperties2(m_physical_devices[m_selected_physical_device], &m_physical_device_properties);
        vkGetPhysicalDeviceFeatures2(m_physical_devices[m_selected_physical_device], &m_physical_device_features);
        vkGetPhysicalDeviceMemoryProperties2(m_physical_devices[m_selected_physical_device], &m_physical_device_memory_properties);

        fill_enumeration([&](uint32_t* count, VkQueueFamilyProperties* data) {
            vkGetPhysicalDeviceQueueFamilyProperties(m_physical_devices[m_selected_physical_device], count, data);
                         }, m_physical_device_queue_family_properties);

    }

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                vulkan);

}

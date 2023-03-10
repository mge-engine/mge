// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/trace.hpp"

#include "dump.hpp"
#include "window.hpp"

#include "enumerate.hpp"
#include "error.hpp"

#include <limits>
#include <memory>

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#endif

namespace mge {
    MGE_USE_TRACE(VULKAN);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, vulkan, debug, "Enable Vulkan debug mode", false);

} // namespace mge

namespace mge::vulkan {

    render_system::render_system()
        : m_instance(VK_NULL_HANDLE)
        , m_debug_messenger(VK_NULL_HANDLE)
        , m_physical_device(VK_NULL_HANDLE)
        , m_graphics_queue(VK_NULL_HANDLE)
        , m_depth_format(VK_FORMAT_UNDEFINED)
    {
        try {
            MGE_INFO_TRACE(VULKAN) << "Creating Vulkan render system";
            m_library = std::make_shared<vulkan_library>();
            resolve_basic_instance_functions();
            fetch_instance_extensions(nullptr, m_instance_extension_properties);
            for (const auto& e : m_instance_extension_properties) {
                m_available_extensions.insert(e.extensionName);
            }
            fetch_layers();
            create_instance();
            pick_physical_device();
            select_queue_families();
            select_depth_format();
        } catch (...) {
            teardown();
            throw;
        }
    }

    render_system::~render_system()
    {
        try {
            teardown();
        } catch (const std::exception& e) {
            MGE_ERROR_TRACE(VULKAN) << "Error in Vulkan teardown: " << e.what();
        }
    }
#ifdef MGE_OS_WINDOWS
    static const char* s_default_extensions[] = {"VK_KHR_surface",
                                                 "VK_KHR_win32_surface"};
#else
#    error Missing port
#endif
    void render_system::fetch_layers()
    {
        enumerate(
            [this](uint32_t* count, VkLayerProperties* data) {
                CHECK_VK_CALL(vkEnumerateInstanceLayerProperties(count, data));
            },
            m_layer_properties);

        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_layer_properties.size() << " Vulkan layers";
        for (const auto& l : m_layer_properties) {
            MGE_DEBUG_TRACE(VULKAN) << details(l);
            std::vector<VkExtensionProperties> extension_properties;
            fetch_instance_extensions(l.layerName, extension_properties);
            m_layer_extensions[l.layerName] = std::move(extension_properties);
        }
    }

    void render_system::fetch_instance_extensions(
        const char* layer, std::vector<VkExtensionProperties>& properties)
    {
        enumerate(
            [this, layer](uint32_t* count, VkExtensionProperties* data) {
                CHECK_VK_CALL(
                    vkEnumerateInstanceExtensionProperties(layer, count, data));
            },
            properties);
        MGE_DEBUG_TRACE(VULKAN) << "Found " << properties.size()
                                << " Vulkan instance extensions (layer: "
                                << (layer ? layer : "default") << ")";
        for (const auto& p : properties) {
            MGE_DEBUG_TRACE(VULKAN) << details(p);
        }
    }

    static void*
    resolve_basic_instance_function(PFN_vkGetInstanceProcAddr getInstanceProc,
                                    const char*               name)
    {
        auto ptr = getInstanceProc(VK_NULL_HANDLE, name);
        if (!ptr) {
            MGE_THROW(vulkan::error)
                << "Cannot resolve instance function: " << name;
        }
        MGE_DEBUG_TRACE(VULKAN) << "Resolve " << name << ": " << (void*)ptr;
        return ptr;
    }

    void render_system::resolve_basic_instance_functions()
    {
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    this->X = reinterpret_cast<decltype(this->X)>(                             \
        resolve_basic_instance_function(m_library->vkGetInstanceProcAddr,      \
                                        #X));

#define BASIC_INSTANCE_FUNCTION(X) RESOLVE(X)
#define INSTANCE_FUNCTION(X)
#define DEVICE_FUNCTION(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    static void*
    resolve_instance_function(PFN_vkGetInstanceProcAddr getInstanceProc,
                              VkInstance                instance,
                              const char*               name)
    {
        auto ptr = getInstanceProc(instance, name);
        MGE_DEBUG_TRACE(VULKAN) << "Resolve " << name << ": " << (void*)ptr;
        return ptr;
    }

    void render_system::resolve_instance_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Resolve instance functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    this->X = reinterpret_cast<decltype(this->X)>(                             \
        resolve_instance_function(m_library->vkGetInstanceProcAddr,            \
                                  m_instance,                                  \
                                  #X));
#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_system::clear_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Clear functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X) this->X = nullptr;

#define BASIC_INSTANCE_FUNCTION(X) RESOLVE(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
#define DEVICE_FUNCTION(X) RESOLVE(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION
#undef RESOLVE
#ifdef MGE_COMPILER_MSVC
#    pragma warning(pop)
#endif
    }

    void render_system::create_instance()
    {
        auto exe_name = mge::executable_name();

        VkApplicationInfo app_info = {};
        app_info.pApplicationName = exe_name.c_str();
        // app_info.applicationVersion = ...
        app_info.pEngineName = "mge";
        // app_info.engineVersion = ...
        app_info.apiVersion = VK_API_VERSION_1_3;

        std::vector<const char*> extensions;
        std::vector<const char*> layers;
        for (const auto& e : s_default_extensions) {
            extensions.push_back(e);
        }

        if (debug()) {
            extensions.push_back("VK_EXT_debug_utils");
            extensions.push_back("VK_EXT_debug_report");
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        MGE_DEBUG_TRACE(VULKAN) << "Selected instance extensions:";
        for (const auto& e : extensions) {
            MGE_DEBUG_TRACE(VULKAN) << "  " << e;
        }

        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
        create_info.ppEnabledLayerNames = layers.data();

        VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info = {};
        if (debug()) {
            debug_utils_create_info.sType =
                VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debug_utils_create_info.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debug_utils_create_info.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debug_utils_create_info.pfnUserCallback = &debug_message_callback;
            create_info.pNext = &debug_utils_create_info;
        }

        CHECK_VK_CALL(vkCreateInstance(&create_info, nullptr, &m_instance));
        resolve_instance_functions();

        if (debug()) {
            MGE_DEBUG_TRACE(VULKAN) << "Enable extra debug messages";
            init_debug_messenger();
        }
    }

    VkBool32 render_system::debug_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* data,
        void*                                       userdata)
    {
        MGE_DEBUG_TRACE(VULKAN) << data->pMessage;
        return VK_FALSE; // do not abort the call
    }

    void render_system::init_debug_messenger()
    {
        if (!vkCreateDebugUtilsMessengerEXT) {
            MGE_DEBUG_TRACE(VULKAN)
                << "Vulkan debug utils extension unavailable";
            return;
        }
        VkDebugUtilsMessengerCreateInfoEXT create_info = {};
        create_info.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = &debug_message_callback;
        CHECK_VK_CALL(vkCreateDebugUtilsMessengerEXT(m_instance,
                                                     &create_info,
                                                     nullptr,
                                                     &m_debug_messenger));
    }

    void render_system::pick_physical_device()
    {
        enumerate(
            [this](uint32_t* count, VkPhysicalDevice* data) {
                CHECK_VK_CALL(
                    vkEnumeratePhysicalDevices(m_instance, count, data));
            },
            m_all_physical_devices);

        if (m_all_physical_devices.size() == 1) {
            MGE_DEBUG_TRACE(VULKAN) << "Found 1 physical device";
        } else {
            MGE_DEBUG_TRACE(VULKAN) << "Found " << m_all_physical_devices.size()
                                    << " physical devices";
        }
        if (m_all_physical_devices.empty()) {
            MGE_THROW(error) << "No physical devices found";
        }

        m_all_physical_device_properties.resize(m_all_physical_devices.size());
        m_all_physical_device_features.resize(m_all_physical_devices.size());
        for (size_t i = 0; i < m_all_physical_devices.size(); ++i) {
            vkGetPhysicalDeviceProperties(m_all_physical_devices[i],
                                          &m_all_physical_device_properties[i]);
            vkGetPhysicalDeviceFeatures(m_all_physical_devices[i],
                                        &m_all_physical_device_features[i]);
            MGE_DEBUG_TRACE(VULKAN)
                << "Physical Device #" << i << ": "
                << details(m_all_physical_device_properties[i]);

            if (m_physical_device == VK_NULL_HANDLE) {
                if (m_all_physical_device_properties[i].deviceType ==
                    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                    m_physical_device = m_all_physical_devices[i];
                }
            }
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            MGE_THROW(error) << "No suitable physical device found";
        }
    }

    void render_system::select_queue_families()
    {
        enumerate(
            [this](uint32_t* count, VkQueueFamilyProperties* data) {
                vkGetPhysicalDeviceQueueFamilyProperties(physical_device(),
                                                         count,
                                                         data);
            },
            m_queue_families);

        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_queue_families.size() << " queue families";

        for (size_t i = 0; i < m_queue_families.size(); ++i) {
            if (m_queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphics_family = checked_cast<uint32_t>(i);
                break;
            }
        }

        if (!m_graphics_family.has_value()) {
            MGE_THROW(error) << "No graphics queue family found";
        }
    }

    void render_system::select_depth_format()
    {
        const VkFormat formats[] = {VK_FORMAT_D32_SFLOAT_S8_UINT,
                                    VK_FORMAT_D32_SFLOAT,
                                    VK_FORMAT_D24_UNORM_S8_UINT,
                                    VK_FORMAT_D16_UNORM_S8_UINT,
                                    VK_FORMAT_D16_UNORM};

        for (auto& format : formats) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physical_device,
                                                format,
                                                &props);
            if (props.optimalTilingFeatures &
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
                m_depth_format = format;
                break;
            }
        }

        if (m_depth_format == VK_FORMAT_UNDEFINED) {
            MGE_THROW(vulkan::error) << "No suitable depth buffer format found";
        }
    }

    void render_system::destroy_instance()
    {
        if (m_debug_messenger) {
            vkDestroyDebugUtilsMessengerEXT(m_instance,
                                            m_debug_messenger,
                                            nullptr);
            m_debug_messenger = VK_NULL_HANDLE;
        }

        vkDestroyInstance(m_instance, nullptr);
        m_instance = nullptr;
        clear_functions();
    }

    void render_system::teardown()
    {

        m_physical_device = VK_NULL_HANDLE;
        m_all_physical_devices.clear();
        m_all_physical_device_properties.clear();
        m_all_physical_device_features.clear();

        if (m_instance && vkDestroyInstance) {
            destroy_instance();
        }
    }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        return std::make_shared<mge::vulkan::window>(*this, extent, options);
    }

    mge::render_system::monitor_collection render_system::monitors()
    {
#ifdef MGE_OS_WINDOWS
        return mge::win32::monitor::all_monitors();
#else
#    error Missing Port
#endif
    }

    bool render_system::debug() const
    {
        return true; // return MGE_PARAMETER(vulkan, debug).get();
    }

    uint32_t render_system::graphics_queue_family_index() const
    {
        if (!m_graphics_family.has_value()) {
            MGE_THROW(vulkan::error) << "No graphics queue family present";
        }
        return m_graphics_family.value();
    }

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan
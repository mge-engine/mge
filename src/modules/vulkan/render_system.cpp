// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "error.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"

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
    {
        try {
            m_library = std::make_shared<vulkan_library>();
            resolve_basic_instance_functions();
            resolve_layer_properties();
            create_instance();
        } catch (...) {
            teardown();
            throw;
        }
    }

#ifdef MGE_OS_WINDOWS
    static const char* s_default_extensions[] = {"VK_KHR_surface",
                                                 "VK_KHR_win32_surface"};
#else
#    error Missing port
#endif

    VkBool32 render_system::debug_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* data,
        void*                                       userdata)
    {
        // TODO: more detailed debug message reporting
        switch (severity) {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            MGE_DEBUG_TRACE(VULKAN) << data->pMessage;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            MGE_INFO_TRACE(VULKAN) << data->pMessage;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            MGE_WARNING_TRACE(VULKAN) << data->pMessage;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            MGE_ERROR_TRACE(VULKAN) << data->pMessage;
            break;
        }

        return VK_FALSE; // TODO: check when call shall be aborted
    }

    void render_system::create_instance()
    {
        auto application_name = mge::executable_name();

        VkApplicationInfo application_info = {};
        application_info.pApplicationName = application_name.c_str();
        // TODO: manage application version
        // app_info.applicationVersion = ...
        application_info.pEngineName = "mge";
        // TODO: manage engine (library version)
        // app_info.engineVersion = ...
        application_info.apiVersion = VK_API_VERSION_1_3;

        // TODO: manage instance layers and extensions externally
        std::vector<const char*> extensions;
        std::vector<const char*> layers;
        for (const auto& e : s_default_extensions) {
            extensions.push_back(e);
        }

        if (debug()) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        VkInstanceCreateInfo instance_create_info = {};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &application_info;
        instance_create_info.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        instance_create_info.ppEnabledExtensionNames = extensions.data();
        instance_create_info.enabledLayerCount =
            static_cast<uint32_t>(layers.size());
        instance_create_info.ppEnabledLayerNames = layers.data();

        // need to create always as it has to be in scope
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
        if (debug()) {
            debug_create_info.sType =
                VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debug_create_info.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debug_create_info.messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debug_create_info.pfnUserCallback = debug_message_callback;
            instance_create_info.pNext = &debug_create_info;
        }

        // TODO: manage memory allocation for instance
        CHECK_VK_CALL(
            vkCreateInstance(&instance_create_info, nullptr, &m_instance));
        resolve_instance_functions();
    }

    void render_system::resolve_layer_properties()
    {
        uint32_t count = 0;
        CHECK_VK_CALL(vkEnumerateInstanceLayerProperties(&count, nullptr));
        m_layer_properties.resize(count);
        CHECK_VK_CALL(
            vkEnumerateInstanceLayerProperties(&count,
                                               m_layer_properties.data()));

        MGE_DEBUG_TRACE(VULKAN) << "Found " << count << " layers:";
        for (const auto& layer : m_layer_properties) {
            MGE_DEBUG_TRACE(VULKAN)
                << "  " << layer.layerName << ": " << layer.description;
            uint32_t extension_count = 0;
            CHECK_VK_CALL(
                vkEnumerateInstanceExtensionProperties(layer.layerName,
                                                       &extension_count,
                                                       nullptr));
            if (extension_count) {
                std::vector<VkExtensionProperties> properties(extension_count);
                CHECK_VK_CALL(
                    vkEnumerateInstanceExtensionProperties(layer.layerName,
                                                           &extension_count,
                                                           properties.data()));
                MGE_DEBUG_TRACE(VULKAN) << "    Extensions:";
                for (const auto& prop : properties) {
                    MGE_DEBUG_TRACE(VULKAN) << "      " << prop.extensionName;
                }
                m_instance_extensions[layer.layerName] = std::move(properties);
            }
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
        MGE_DEBUG_TRACE(VULKAN)
            << "Resolve instance proc: " << name << ": " << (void*)ptr;
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

    void render_system::teardown()
    {
        m_instance_extensions.clear();
        m_layer_properties.clear();
        destroy_instance();
        m_library.reset();
    }

    render_system::~render_system() { teardown(); }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        return nullptr;
    }

    std::span<mge::monitor_ref> render_system::monitors()
    {
#ifdef MGE_OS_WINDOWS
        return mge::win32::monitor::all_monitors();
#else
#    error Missing Port
#endif
    }

    bool render_system::debug() const
    {
        const char* vulkan_debug_env = std::getenv("MGE_VULKAN_DEBUG");
        bool        result = MGE_PARAMETER(vulkan, debug).get(false);
        if (!result && vulkan_debug_env != nullptr) {
            return std::string(vulkan_debug_env) == "1" ||
                   std::string(vulkan_debug_env) == "true";
        }
        return result;
    }

    void render_system::destroy_instance()
    {
        if (m_instance != VK_NULL_HANDLE && vkDestroyInstance != nullptr) {
            vkDestroyInstance(m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
        }
    }

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan
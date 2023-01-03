// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/trace.hpp"
#include "window.hpp"

// #include "window.hpp"
#include "error.hpp"
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
    {
        try {
            MGE_INFO_TRACE(VULKAN) << "Creating Vulkan render system";
            m_library = std::make_shared<vulkan_library>();
            resolve_basic_instance_functions();
            fetch_instance_extensions(nullptr);
            fetch_layers();
            create_instance();
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
        uint32_t property_count = 0;
        CHECK_VK_CALL(
            vkEnumerateInstanceLayerProperties(&property_count, nullptr));
        std::vector<VkLayerProperties> layer_properties;
        layer_properties.resize(property_count);
        CHECK_VK_CALL(
            vkEnumerateInstanceLayerProperties(&property_count,
                                               layer_properties.data()));
        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << property_count << " Vulkan layers";
        for (const auto& p : layer_properties) {
            MGE_DEBUG_TRACE(VULKAN) << "Found layer: " << p.layerName << " ("
                                    << p.description << ")";
            fetch_instance_extensions(p.layerName);
        }
    }

    void render_system::fetch_instance_extensions(const char* layer)
    {
        uint32_t property_count = 0;
        CHECK_VK_CALL(vkEnumerateInstanceExtensionProperties(layer,
                                                             &property_count,
                                                             nullptr));
        if (property_count == 0) {
            return;
        }
        std::vector<VkExtensionProperties> extension_properties;
        extension_properties.resize(property_count);
        CHECK_VK_CALL(vkEnumerateInstanceExtensionProperties(
            layer,
            &property_count,
            extension_properties.data()));
        MGE_DEBUG_TRACE(VULKAN) << "Found " << property_count
                                << " Vulkan instance extensions (layer: "
                                << (layer ? layer : "default") << ")";
        for (const auto& p : extension_properties) {
            MGE_DEBUG_TRACE(VULKAN) << "Extension found: " << p.extensionName;
            if (layer == nullptr) {
                m_available_extensions.insert(p.extensionName);
            }
        }
    }

    void render_system::resolve_basic_instance_functions()
    {
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    do {                                                                       \
        auto f = m_library->vkGetInstanceProcAddr(VK_NULL_HANDLE, #X);         \
        if (!f) {                                                              \
            MGE_THROW(vulkan::error)                                           \
                << "Cannot resolve instance function: " << #X;                 \
        }                                                                      \
        MGE_DEBUG_TRACE(VULKAN) << "Resolve " << #X << ": " << (void*)f;       \
        this->X = reinterpret_cast<decltype(this->X)>(f);                      \
    } while (false);

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

    void render_system::resolve_instance_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Resolve instance functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    do {                                                                       \
        auto f = m_library->vkGetInstanceProcAddr(m_instance, #X);             \
        MGE_DEBUG_TRACE(VULKAN) << "Resolve " << #X << ": " << (void*)f;       \
        this->X = reinterpret_cast<decltype(this->X)>(f);                      \
    } while (false);

#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X) RESOLVE(X)
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
        for (const auto& e : s_default_extensions) {
            extensions.push_back(e);
        }

        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;
        create_info.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.enabledLayerCount = 0;
        create_info.ppEnabledLayerNames = nullptr;
        if (debug()) {
            MGE_DEBUG_TRACE(VULKAN) << "Enable extra debug messages";
        }

        CHECK_VK_CALL(vkCreateInstance(&create_info, nullptr, &m_instance));
        resolve_instance_functions();
    }

    void render_system::destroy_instance()
    {
        vkDestroyInstance(m_instance, nullptr);
        m_instance = nullptr;
        clear_functions();
    }

    void render_system::teardown()
    {
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
        return MGE_PARAMETER(vulkan, debug).get();
    }

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan
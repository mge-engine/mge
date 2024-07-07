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

    void render_system::create_instance()
    {
        auto application_name = mge::executable_name();

        VkApplicationInfo app_info = {};
        app_info.pApplicationName = exe_name.c_str();
        // TODO: manage application version
        // app_info.applicationVersion = ...
        app_info.pEngineName = "mge";
        // TODO: manage engine (library version)
        // app_info.engineVersion = ...
        app_info.apiVersion = VK_API_VERSION_1_3;

        std::vector<const char*> extensions;
        std::vector<const char*> layers;
        for (const auto& e : s_default_extensions) {
            extensions.push_back(e);
        }
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

    void render_system::teardown() { m_library.reset(); }

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
}

MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan
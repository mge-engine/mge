// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "glslang.hpp"
#include "window.hpp"

#include "mge/core/atexit.hpp"
#include "mge/core/executable_name.hpp"
#include "mge/core/iterator_index.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"

#ifdef MGE_OS_WINDOWS
#    include "mge/win32/monitor.hpp"
#elif defined MGE_OS_MACOSX
#    include "mge/glfw/monitor.hpp"
#    include <GLFW/glfw3.h>
#endif

template <> struct fmt::formatter<VkDebugUtilsMessageTypeFlagBitsEXT>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }
    template <typename FormatContext>
    auto format(VkDebugUtilsMessageTypeFlagBitsEXT flags, FormatContext& ctx)
    {
        bool first = true;
        if (flags & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
            fmt::format_to(ctx.out(), "GENERAL");
            first = false;
        }
        if (flags & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
            if (!first)
                fmt::format_to(ctx.out(), "|");
            fmt::format_to(ctx.out(), "VALIDATION");
            first = false;
        }
        if (flags & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
            if (!first)
                fmt::format_to(ctx.out(), "|");
            fmt::format_to(ctx.out(), "PERFORMANCE");
        }
        return ctx.out();
    }
};

namespace mge {
    MGE_USE_TRACE(VULKAN);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(
        bool, vulkan, debug, "Enable Vulkan debug mode", true);
    MGE_DEFINE_PARAMETER_WITH_DEFAULT(bool,
                                      vulkan,
                                      stop_on_validation_error,
                                      "Stop on Vulkan validation errors",
                                      true);
} // namespace mge

namespace mge::vulkan {

    bool render_system::s_glslang_initialized = false;

    render_system::render_system()
    {
        try {
            MGE_INFO_TRACE(VULKAN, "Creating Vulkan render system");
            init_capabilities();
            m_library = std::make_shared<vulkan_library>();
#ifdef MGE_OS_MACOSX
            glfwInitVulkanLoader(m_library->vkGetInstanceProcAddr);
            if (!glfwInit()) {
                MGE_THROW(mge::runtime_exception) << "glfwInit failed";
            }
#endif
            resolve_basic_instance_functions();
            resolve_layer_properties();
            create_instance();
            pick_physical_device();
            select_queue_families();
            init_glslang();
        } catch (...) {
            teardown();
            throw;
        }
    }

    void render_system::init_glslang()
    {
        if (!s_glslang_initialized) {
            MGE_DEBUG_TRACE(VULKAN, "Initializing shader compiler");
            auto rc = glslang_initialize_process();
            if (!rc) {
                MGE_THROW(vulkan::error)
                    << "Failed to initialize shader compiler: " << rc;
            }
            s_glslang_initialized = true;
        }
    }

    void render_system::init_capabilities()
    {
        class capabilities : public mge::render_system::capabilities
        {
        public:
            capabilities()
            {
                shader_language glsl{"glsl"sv, mge::semantic_version(4, 6)};
                m_shader_languages.push_back(glsl);
                m_shader_languages.push_back(shader_language::SLANG);
                shader_format spirv{"spirv"sv, mge::semantic_version(1, 5)};
                m_shader_formats.push_back(spirv);
            }
            ~capabilities() = default;

            std::span<const mge::shader_language>
            shader_languages() const override
            {
                return std::span<const mge::shader_language>(
                    m_shader_languages.data(),
                    m_shader_languages.size());
            }

            std::span<const mge::shader_format> shader_formats() const override
            {
                return std::span<const mge::shader_format>(
                    m_shader_formats.data(),
                    m_shader_formats.size());
            }

        private:
            std::vector<mge::shader_language> m_shader_languages;
            std::vector<mge::shader_format>   m_shader_formats;
        };

        m_capabilities = std::make_unique<capabilities>();
    }

    render_system::~render_system()
    {
        try {
            teardown();
        } catch (const std::exception& e) {
            MGE_ERROR_TRACE(VULKAN, "Error in Vulkan teardown: {}", e.what());
        }
    }
#ifdef MGE_OS_WINDOWS
    static const char* s_default_extensions[] = {"VK_KHR_surface",
                                                 "VK_KHR_win32_surface"};
#elif defined MGE_OS_MACOSX
    static const char** s_glfw_extensions = nullptr;
    static uint32_t     s_glfw_extension_count = 0;

    static const char** get_default_extensions(uint32_t& count)
    {
        s_glfw_extensions =
            glfwGetRequiredInstanceExtensions(&s_glfw_extension_count);
        if (!s_glfw_extensions) {
            MGE_THROW(mge::runtime_exception)
                << "glfwGetRequiredInstanceExtensions failed";
        }
        count = s_glfw_extension_count;
        return s_glfw_extensions;
    }
#else
#    error Missing port
#endif

    VkBool32 render_system::debug_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
        VkDebugUtilsMessageTypeFlagsEXT             type,
        const VkDebugUtilsMessengerCallbackDataEXT* data,
        void*                                       userdata)
    {
        bool stop_on_error =
            MGE_PARAMETER(vulkan, stop_on_validation_error).get();
        bool is_error = false;

        // Format message with all available information
        std::stringstream ss;
        ss << "Vulkan Debug [";

        const char* severity_str = "UNKNOWN";
        // Add severity
        switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            severity_str = "VERBOSE";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            severity_str = "INFO";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            severity_str = "WARNING";
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            severity_str = "ERROR";
            is_error = true;
            break;
        default:
            severity_str = "UNKNOWN";
            break;
        }

        // Log based on severity
        switch (severity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            MGE_DEBUG_TRACE(VULKAN,
                            "Vulkan Debug [{}] ({}) Object: {} ({}): {}",
                            severity_str,
                            type,
                            data->objectCount ? data->pObjects[0].objectType
                                              : VK_OBJECT_TYPE_UNKNOWN,
                            data->objectCount ? data->pObjects[0].objectHandle
                                              : 0,
                            data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            MGE_INFO_TRACE(VULKAN,
                           "Vulkan Debug [{}] ({}) Object: {} ({}): {}",
                           severity_str,
                           type,
                           data->objectCount ? data->pObjects[0].objectType
                                             : VK_OBJECT_TYPE_UNKNOWN,
                           data->objectCount ? data->pObjects[0].objectHandle
                                             : 0,
                           data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            MGE_WARNING_TRACE(VULKAN,
                              "Vulkan Debug [{}] ({}) Object: {} ({}): {}",
                              severity_str,
                              type,
                              data->objectCount ? data->pObjects[0].objectType
                                                : VK_OBJECT_TYPE_UNKNOWN,
                              data->objectCount ? data->pObjects[0].objectHandle
                                                : 0,
                              data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            MGE_ERROR_TRACE(VULKAN,
                            "Vulkan Debug [{}] ({}) Object: {} ({}): {}",
                            severity_str,
                            type,
                            data->objectCount ? data->pObjects[0].objectType
                                              : VK_OBJECT_TYPE_UNKNOWN,
                            data->objectCount ? data->pObjects[0].objectHandle
                                              : 0,
                            data->pMessage);
            break;
        default:
            MGE_INFO_TRACE(VULKAN,
                           "Vulkan Debug [{}] ({}) Object: {} ({}): {}",
                           severity_str,
                           type,
                           data->objectCount ? data->pObjects[0].objectType
                                             : VK_OBJECT_TYPE_UNKNOWN,
                           data->objectCount ? data->pObjects[0].objectHandle
                                             : 0,
                           data->pMessage);
            break;
        }

        if (is_error && stop_on_error) {
            return VK_TRUE;
        } else {
            return VK_FALSE;
        }
    }

    void render_system::create_instance()
    {
        auto application_name = mge::executable_name();

        VkApplicationInfo application_info = {};
        application_info.pApplicationName = application_name.c_str();

        // app_info.applicationVersion = ...
        application_info.pEngineName = "mge";
        // app_info.engineVersion = ...
        application_info.apiVersion = VK_API_VERSION_1_3;

        // : manage instance layers and extensions externally
        std::vector<const char*> extensions;
        std::vector<const char*> layers;
#ifdef MGE_OS_WINDOWS
        for (const auto& e : s_default_extensions) {
            extensions.push_back(e);
        }
#elif defined MGE_OS_MACOSX
        uint32_t     ext_count = 0;
        const char** exts = get_default_extensions(ext_count);
        for (uint32_t i = 0; i < ext_count; ++i) {
            extensions.push_back(exts[i]);
        }
#endif

        if (debug()) {
            MGE_DEBUG_TRACE(VULKAN,
                            "Enabling Vulkan instance debug extensions");
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            auto validation_layers = select_validation_layers();
            for (const auto& l : validation_layers) {
                layers.push_back(l);
            }
        }

#ifdef MGE_OS_MACOSX
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

        VkInstanceCreateInfo instance_create_info = {};
        instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info.pApplicationInfo = &application_info;
#ifdef MGE_OS_MACOSX
        instance_create_info.flags =
            VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        instance_create_info.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        instance_create_info.ppEnabledExtensionNames = extensions.data();
        instance_create_info.enabledLayerCount =
            static_cast<uint32_t>(layers.size());
        instance_create_info.ppEnabledLayerNames = layers.data();

        // need to create always as it has to be in scope
        VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {};
        if (debug()) {
            MGE_DEBUG_TRACE(VULKAN, "Attaching debug message callback");
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

        CHECK_VK_CALL(
            vkCreateInstance(&instance_create_info, nullptr, &m_instance));
        resolve_instance_functions();
        if (debug() && vkCreateDebugUtilsMessengerEXT) {
            CHECK_VK_CALL(vkCreateDebugUtilsMessengerEXT(m_instance,
                                                         &debug_create_info,
                                                         nullptr,
                                                         &m_debug_messenger));
        }
    }

    bool render_system::has_layer(const char* name) const
    {
        for (const auto& layer : m_layer_properties) {
            if (strcmp(layer.layerName, name) == 0) {
                return true;
            }
        }
        return false;
    }

    std::vector<const char*> render_system::select_validation_layers() const
    {
        // Prio 1: unified Khronos validation layer
        if (has_layer("VK_LAYER_KHRONOS_validation")) {
            MGE_INFO_TRACE(VULKAN,
                           "Using validation layer: "
                           "VK_LAYER_KHRONOS_validation");
            return {"VK_LAYER_KHRONOS_validation"};
        }
        // Prio 2: LunarG standard validation meta-layer
        if (has_layer("VK_LAYER_LUNARG_standard_validation")) {
            MGE_INFO_TRACE(VULKAN,
                           "Using validation layer: "
                           "VK_LAYER_LUNARG_standard_validation");
            return {"VK_LAYER_LUNARG_standard_validation"};
        }
        // Prio 3: individual layers that make up the old standard set
        static const char* s_prio3_layers[] = {
            "VK_LAYER_GOOGLE_threading",
            "VK_LAYER_LUNARG_parameter_validation",
            "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation",
            "VK_LAYER_GOOGLE_unique_objects"};
        bool all_prio3 = true;
        for (const auto* l : s_prio3_layers) {
            if (!has_layer(l)) {
                all_prio3 = false;
                break;
            }
        }
        if (all_prio3) {
            MGE_INFO_TRACE(VULKAN,
                           "Using individual validation layers (prio 3 set)");
            return {s_prio3_layers[0],
                    s_prio3_layers[1],
                    s_prio3_layers[2],
                    s_prio3_layers[3],
                    s_prio3_layers[4]};
        }
        // Prio 4: core validation only
        if (has_layer("VK_LAYER_LUNARG_core_validation")) {
            MGE_INFO_TRACE(VULKAN,
                           "Using validation layer: "
                           "VK_LAYER_LUNARG_core_validation");
            return {"VK_LAYER_LUNARG_core_validation"};
        }
        MGE_WARNING_TRACE(VULKAN, "No Vulkan validation layers available");
        return {};
    }

    void render_system::resolve_layer_properties()
    {
        uint32_t count = 0;
        CHECK_VK_CALL(vkEnumerateInstanceLayerProperties(&count, nullptr));
        m_layer_properties.resize(count);
        CHECK_VK_CALL(
            vkEnumerateInstanceLayerProperties(&count,
                                               m_layer_properties.data()));

        MGE_DEBUG_TRACE(VULKAN, "Found {} layers:", count);
        for (const auto& layer : m_layer_properties) {
            MGE_DEBUG_TRACE(VULKAN,
                            "  {}: {}",
                            layer.layerName,
                            layer.description);
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
                MGE_DEBUG_TRACE(VULKAN, "    Extensions:");
                for (const auto& prop : properties) {
                    MGE_DEBUG_TRACE(VULKAN, "      {}", prop.extensionName);
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
        // Too much output
        // MGE_DEBUG_TRACE(VULKAN)
        //     << "Resolve instance proc: " << name << ": " << (void*)ptr;
        return reinterpret_cast<void*>(ptr);
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
        // MGE_DEBUG_TRACE(VULKAN) << "Resolve " << name << ": " << (void*)ptr;
        return reinterpret_cast<void*>(ptr);
    }

    void render_system::resolve_instance_functions()
    {
        MGE_DEBUG_TRACE(VULKAN, "Resolve instance functions");
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
        if (s_glslang_initialized) {
            MGE_DEBUG_TRACE(VULKAN, "Finalizing shader compiler");
            glslang_finalize_process();
            s_glslang_initialized = false;
        }
        m_graphics_queue_index = 0;
        m_queue_family_properties.clear();
        m_physical_device = VK_NULL_HANDLE;
        m_physical_device_features.clear();
        m_physical_device_properties.clear();
        m_all_physical_devices.clear();
        destroy_instance();
        m_instance_extensions.clear();
        m_layer_properties.clear();
        m_library.reset();
    }

    mge::window_ref
    render_system::create_window(const mge::extent&         extent,
                                 const mge::window_options& options)
    {
        return std::make_shared<window>(*this, extent, options);
    }

    std::span<mge::monitor_ref> render_system::monitors()
    {
#ifdef MGE_OS_WINDOWS
        return mge::win32::monitor::all_monitors();
#elif defined MGE_OS_MACOSX
        return mge::glfw::monitor::all_monitors();
#else
#    error Missing Port
#endif
    }

    bool render_system::debug() const
    {
        return MGE_PARAMETER(vulkan, debug).get();
    }

    bool render_system::stop_on_validation_errors() const
    {
        return MGE_PARAMETER(vulkan, stop_on_validation_error).get();
    }

    void render_system::destroy_instance()
    {
        if (m_debug_messenger && vkDestroyDebugUtilsMessengerEXT) {
            vkDestroyDebugUtilsMessengerEXT(m_instance,
                                            m_debug_messenger,
                                            nullptr);
            m_debug_messenger = VK_NULL_HANDLE;
        }

        if (m_instance != VK_NULL_HANDLE && vkDestroyInstance != nullptr) {
            vkDestroyInstance(m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
        }
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
            MGE_DEBUG_TRACE(VULKAN, "Found 1 physical device");
        } else {
            MGE_DEBUG_TRACE(VULKAN,
                            "Found {} physical devices",
                            m_all_physical_devices.size());
        }
        if (m_all_physical_devices.empty()) {
            MGE_THROW(error) << "No physical devices found";
        }

        for (const auto& device : m_all_physical_devices) {
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures   features;
            vkGetPhysicalDeviceProperties(device, &properties);
            vkGetPhysicalDeviceFeatures(device, &features);
            MGE_DEBUG_TRACE(VULKAN,
                            "Physical device: {}",
                            static_cast<void*>(device));
            MGE_DEBUG_TRACE(VULKAN,
                            "    Device name: {}",
                            properties.deviceName);
            MGE_DEBUG_TRACE(VULKAN,
                            "    Device type: {}",
                            properties.deviceType);

            m_physical_device_properties[device] = properties;
            m_physical_device_features[device] = features;
            if (m_physical_device == VK_NULL_HANDLE) {
                if (properties.deviceType ==
                    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                    m_physical_device = device;
                }
            }
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            for (const auto& device : m_all_physical_devices) {
                if (m_physical_device_properties[device].deviceType ==
                    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
                    m_physical_device = device;
                    break;
                }
            }
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            MGE_THROW(error) << "No suitable GPU found to use as Vulkan device";
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
            m_queue_family_properties);
        MGE_DEBUG_TRACE(VULKAN,
                        "Found {} queue families",
                        m_queue_family_properties.size());

        if (m_queue_family_properties.empty()) {
            MGE_THROW(error) << "No queue families found";
        }
        m_graphics_queue_index = m_queue_family_properties.size();
        for (size_t index = 0; index < m_queue_family_properties.size();
             ++index) {
            const auto& qf = m_queue_family_properties[index];
            MGE_DEBUG_TRACE(VULKAN,
                            "Queue family {}: {} queues",
                            index,
                            qf.queueCount);
            MGE_DEBUG_TRACE(VULKAN,
                            "    Graphics: {}",
                            (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "yes"
                                                                    : "no");
            MGE_DEBUG_TRACE(VULKAN,
                            "    Compute: {}",
                            (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) ? "yes"
                                                                   : "no");
            MGE_DEBUG_TRACE(VULKAN,
                            "    Transfer: {}",
                            (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) ? "yes"
                                                                    : "no");
            MGE_DEBUG_TRACE(
                VULKAN,
                "    Sparse binding: {}",
                (qf.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "yes" : "no");
            if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
                m_graphics_queue_index == m_queue_family_properties.size()) {
                m_graphics_queue_index = index;
            }
            if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
                m_graphics_queue_index == m_queue_family_properties.size()) {
                m_graphics_queue_index = index;
            }
        }
        if (m_graphics_queue_index == m_queue_family_properties.size()) {
            MGE_THROW(error) << "No graphics queue family found";
        }
    }

    void* render_system::renderdoc_device() const
    {
        if (m_instance != VK_NULL_HANDLE) {
#define INSTANCE_FOR_RENDERDOC(inst) (*((void**)(inst)))
            return INSTANCE_FOR_RENDERDOC(m_instance);
#undef INSTANCE_FOR_RENDERDOC
        }
        return nullptr;
    }

    MGE_REGISTER_IMPLEMENTATION(render_system, mge::render_system, vulkan, vk);
} // namespace mge::vulkan
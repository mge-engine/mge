// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "dump.hpp"
#include "enumerate.hpp"
#include "error.hpp"
#include "mge/core/checked_cast.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

#include <limits>

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(::mge::vulkan::render_system& system_,
                                   window&                       window_)
        : m_render_system(system_)
        , m_window(window_)
        , m_surface(VK_NULL_HANDLE)
        , m_device(VK_NULL_HANDLE)
        , m_graphics_queue(VK_NULL_HANDLE)
        , m_present_queue(VK_NULL_HANDLE)
        , m_used_surface_format{.format = VK_FORMAT_UNDEFINED}
        , m_used_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        , m_render_pass(VK_NULL_HANDLE)
        , m_command_pool(VK_NULL_HANDLE)
        , m_frame_finish(VK_NULL_HANDLE)
        , m_render_finished(VK_NULL_HANDLE)
        , m_vulkan_swap_chain(nullptr)
    {
        clear_functions();
    }

    void render_context::initialize()
    {
        try {
            create_surface();
            select_present_queue();
            fetch_surface_capabilities();
            choose_surface_format();
            create_device();
            resolve_device_functions();
            get_graphics_queue();
            get_present_queue();
            create_swap_chain();
            create_render_pass();
            create_frame_buffers();
            create_command_pool();
            create_fence();
            create_semaphores();
        } catch (...) {
            teardown();
            throw;
        }
    }

    render_context::~render_context() { teardown(); }

    void render_context::teardown()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Destroy render context";

        if (m_render_finished) {
            vkDestroySemaphore(m_device, m_render_finished, nullptr);
            m_render_finished = VK_NULL_HANDLE;
        }

        if (m_frame_finish) {
            vkDestroyFence(m_device, m_frame_finish, nullptr);
            m_frame_finish = VK_NULL_HANDLE;
        }

        if (m_command_pool) {
            vkDestroyCommandPool(m_device, m_command_pool, nullptr);
            m_command_pool = VK_NULL_HANDLE;
        }

        if (m_render_pass) {
            vkDestroyRenderPass(m_device, m_render_pass, nullptr);
            m_render_pass = VK_NULL_HANDLE;
        }

        m_vulkan_swap_chain = nullptr;
        m_swap_chain.reset();

        m_graphics_queue = VK_NULL_HANDLE;
        m_present_queue = VK_NULL_HANDLE;

        if (m_device) {
            vkDestroyDevice(m_device, nullptr);
            m_device = VK_NULL_HANDLE;
        }

        if (m_surface) {
            m_render_system.vkDestroySurfaceKHR(m_render_system.instance(),
                                                m_surface,
                                                nullptr);
        }

        clear_functions();
    }

    void render_context::clear_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Clear context functions";
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

    static void* resolve_device_function(void*                   original,
                                         PFN_vkGetDeviceProcAddr getDeviceProc,
                                         VkDevice                device,
                                         const char*             name)
    {
        void* result = original;
        auto  ptr = getDeviceProc(device, name);
        if (ptr) {
            MGE_DEBUG_TRACE(VULKAN)
                << "Replace " << name << ": " << (void*)(original) << " by "
                << (void*)ptr;
            result = ptr;
        }
        return result;
    }

    void render_context::resolve_device_functions()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Resolve device functions";
#ifdef MGE_COMPILER_MSVC
#    pragma warning(push)
#    pragma warning(disable : 4191)
#endif
#define RESOLVE(X)                                                             \
    this->X = reinterpret_cast<decltype(this->X)>(                             \
        resolve_device_function((void*)m_render_system.X,                      \
                                m_render_system.vkGetDeviceProcAddr,           \
                                m_device,                                      \
                                #X));

#define BASIC_INSTANCE_FUNCTION(X)
#define INSTANCE_FUNCTION(X)
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
    } // namespace mge::vulkan

    void render_context::create_render_pass()
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = m_used_surface_format.format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;
        render_pass_info.dependencyCount = 1;
        render_pass_info.pDependencies = &dependency;

        CHECK_VK_CALL(vkCreateRenderPass(m_device,
                                         &render_pass_info,
                                         nullptr,
                                         &m_render_pass));
    }

    void render_context::create_surface()
    {
#ifdef MGE_OS_WINDOWS
        MGE_DEBUG_TRACE(VULKAN) << "Create Vulkan surface";
        if (!m_render_system.vkCreateWin32SurfaceKHR) {
            MGE_THROW(vulkan::error)
                << "Cannot create surface: vkCreateWin32SurfaceKHR function "
                   "missing";
        }
        VkWin32SurfaceCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.hinstance = GetModuleHandle(nullptr);
        create_info.hwnd = m_window.hwnd();
        create_info.flags = 0;

        CHECK_VK_CALL(
            m_render_system.vkCreateWin32SurfaceKHR(m_render_system.instance(),
                                                    &create_info,
                                                    nullptr,
                                                    &m_surface));

#endif
    }

    void render_context::create_device()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Create logical device";

        VkDeviceQueueCreateInfo queue_create_info[2];
        uint32_t                queue_create_count = 2;

        queue_create_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info[0].queueFamilyIndex =
            m_render_system.graphics_queue_family_index();
        queue_create_info[0].queueCount = 1;
        float priority = 1.0f;
        queue_create_info[0].pQueuePriorities = &priority;
        queue_create_info[0].pNext = nullptr;

        if (present_queue_family_index() ==
            m_render_system.graphics_queue_family_index()) {
            MGE_DEBUG_TRACE(VULKAN)
                << "Graphics and present queue are the same";
            queue_create_count = 1;
        } else {
            MGE_DEBUG_TRACE(VULKAN) << "Graphics and present queue differ";
            queue_create_info[1].sType =
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info[1].queueFamilyIndex =
                present_queue_family_index();
            queue_create_info[1].queueCount = 1;
            queue_create_info[1].pQueuePriorities = &priority;
            queue_create_info[1].pNext = nullptr;
        }

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        create_info.pQueueCreateInfos = &queue_create_info[0];
        create_info.queueCreateInfoCount = queue_create_count;

        create_info.pEnabledFeatures = &device_features;

        std::vector<const char*> extensions;
        std::vector<const char*> layers;
        extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        if (m_render_system.debug()) {
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }
        create_info.ppEnabledExtensionNames = extensions.data();
        create_info.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledLayerNames = layers.data();
        create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());

        CHECK_VK_CALL(
            m_render_system.vkCreateDevice(m_render_system.physical_device(),
                                           &create_info,
                                           nullptr,
                                           &m_device));
        MGE_DEBUG_TRACE(VULKAN) << "Created device: " << (void*)m_device;
    }

    void render_context::create_swap_chain()
    {
        auto swc = std::make_shared<mge::vulkan::swap_chain>(*this);
        m_swap_chain = swc;
        m_vulkan_swap_chain = swc.get();
    }

    void render_context::create_frame_buffers()
    {
        auto swap_chain =
            dynamic_cast<mge::vulkan::swap_chain*>(m_swap_chain.get());
        swap_chain->create_frame_buffers(m_render_pass);
    }

    void render_context::create_command_pool()
    {
        VkCommandPoolCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        create_info.queueFamilyIndex =
            m_render_system.graphics_queue_family_index();
        CHECK_VK_CALL(vkCreateCommandPool(m_device,
                                          &create_info,
                                          nullptr,
                                          &m_command_pool));
    }

    void render_context::create_fence()
    {
        VkFenceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        create_info.flags =
            VK_FENCE_CREATE_SIGNALED_BIT; // initially pass through
        CHECK_VK_CALL(
            vkCreateFence(m_device, &create_info, nullptr, &m_frame_finish));
    }

    void render_context::create_semaphores()
    {
        VkSemaphoreCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        CHECK_VK_CALL(vkCreateSemaphore(m_device,
                                        &create_info,
                                        nullptr,
                                        &m_render_finished));
    }

    index_buffer_ref render_context::create_index_buffer(data_type dt,
                                                         size_t    data_size,
                                                         void*     data)
    {
        index_buffer_ref result;
        return result;
    }

    vertex_buffer_ref render_context::create_vertex_buffer(
        const vertex_layout& layout, size_t data_size, void* data)
    {
        vertex_buffer_ref result;
        return result;
    }

    shader_ref render_context::create_shader(shader_type t)
    {
        shader_ref result = std::make_shared<shader>(*this, t);
        return result;
    }

    program_ref render_context::create_program()
    {
        program_ref result;
        return result;
    }

    command_list_ref render_context::create_command_list()
    {
        command_list_ref result =
            std::make_shared<mge::vulkan::command_list>(*this);
        return result;
    }

    void render_context::fetch_surface_capabilities()
    {
        CHECK_VK_CALL(m_render_system.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            m_render_system.physical_device(),
            m_surface,
            &m_surface_capabilities));
        enumerate(
            [this](uint32_t* count, VkSurfaceFormatKHR* data) {
                CHECK_VK_CALL(
                    m_render_system.vkGetPhysicalDeviceSurfaceFormatsKHR(
                        m_render_system.physical_device(),
                        m_surface,
                        count,
                        data));
            },
            m_surface_formats);

        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_surface_formats.size() << " surface formats";
        for (const auto& f : m_surface_formats) {
            MGE_DEBUG_TRACE(VULKAN) << "  " << details(f);
        }
        enumerate(
            [this](uint32_t* count, VkPresentModeKHR* data) {
                CHECK_VK_CALL(
                    m_render_system.vkGetPhysicalDeviceSurfacePresentModesKHR(
                        m_render_system.physical_device(),
                        m_surface,
                        count,
                        data));
            },
            m_surface_present_modes);

        MGE_DEBUG_TRACE(VULKAN)
            << "Found " << m_surface_present_modes.size() << " present modes";
        for (const auto& f : m_surface_present_modes) {
            MGE_DEBUG_TRACE(VULKAN) << "  " << f;
        }
    }

    void render_context::choose_surface_format()
    {
        for (const auto& f : m_surface_formats) {
            if (f.format == VK_FORMAT_B8G8R8A8_SRGB &&
                f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                m_used_surface_format = f;
                return;
            }
        }
        m_used_surface_format = m_surface_formats.at(0);
    }

    VkExtent2D render_context::extent() const
    {
        VkExtent2D result;
        result.height = m_window.extent().height;
        result.width = m_window.extent().width;
        return result;
    }

    void render_context::select_present_queue()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Select present queue";
        const auto queue_families = m_render_system.queue_families();
        for (size_t i = 0; i < queue_families.size(); ++i) {
            VkBool32 present_support = VK_FALSE;
            CHECK_VK_CALL(m_render_system.vkGetPhysicalDeviceSurfaceSupportKHR(
                m_render_system.physical_device(),
                checked_cast<uint32_t>(i),
                m_surface,
                &present_support));
            if (present_support) {
                m_present_family = checked_cast<uint32_t>(i);
                break;
            }
        }

        if (!m_present_family.has_value()) {
            MGE_THROW(vulkan::error) << "Cannot select present queue";
        }
    }

    void render_context::get_present_queue()
    {
        vkGetDeviceQueue(m_device,
                         m_present_family.value(),
                         0u,
                         &m_present_queue);
    }

    void render_context::get_graphics_queue()
    {
        vkGetDeviceQueue(m_device,
                         m_render_system.graphics_queue_family_index(),
                         0,
                         &m_graphics_queue);
    }

    uint32_t render_context::present_queue_family_index() const
    {
        if (!m_present_family.has_value()) {
            MGE_THROW(vulkan::error) << "No present queue family present";
        }
        return m_present_family.value();
    }

    uint32_t render_context::default_image_count() const
    {
        uint32_t result = std::min(m_surface_capabilities.minImageCount + 1,
                                   m_surface_capabilities.maxImageCount);
        return result;
    }

    void render_context::wait_for_frame_completion()
    {
        CHECK_VK_CALL(vkWaitForFences(m_device,
                                      1,
                                      &m_frame_finish,
                                      VK_TRUE,
                                      std::numeric_limits<uint64_t>::max()));
        CHECK_VK_CALL(vkResetFences(m_device, 1, &m_frame_finish));
    }

    void render_context::begin_draw() {}

    void render_context::end_draw() {}

    texture_ref render_context::create_texture(texture_type type)
    {
        texture_ref result;
        return result;
    }

#if 0
    void render_context::execute_on_frame(command_list* l)
    {
        m_frame_commands.emplace_back(l);
    }

    void render_context::frame()
    {
        if (m_frame_commands.empty()) {
            return;
        }
        CHECK_VK_CALL(vkWaitForFences(m_device,
                                      1,
                                      &m_frame_finish,
                                      VK_TRUE,
                                      std::numeric_limits<uint64_t>::max()));
        CHECK_VK_CALL(vkResetFences(m_device, 1, &m_frame_finish));

        VkFramebuffer frame_buffer = m_vulkan_swap_chain->next_frame();
        m_frame_command_buffers.clear();
        m_frame_command_buffers.reserve(m_frame_commands.size());
        for (auto& l : m_frame_commands) {
            l->record_on_frame(frame_buffer);
            m_frame_command_buffers.emplace_back(l->command_buffer());
        }

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore wait_semaphores[] = {
            m_vulkan_swap_chain->image_available()};
        VkPipelineStageFlags wait_stages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = wait_semaphores;
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount =
            checked_cast<uint32_t>(m_frame_command_buffers.size());
        submit_info.pCommandBuffers = m_frame_command_buffers.data();

        VkSemaphore signal_semaphores[] = {m_render_finished};
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = signal_semaphores;

        CHECK_VK_CALL(
            vkQueueSubmit(m_graphics_queue, 1, &submit_info, m_frame_finish));

        m_frame_commands.clear();
    }
#endif

} // namespace mge::vulkan
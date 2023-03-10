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
#include "swap_chain.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(::mge::vulkan::render_system& system_,
                                   window&                       window_)
        : m_render_system(system_)
        , m_window(window_)
        , m_surface(VK_NULL_HANDLE)
        , m_used_present_mode(VK_PRESENT_MODE_FIFO_KHR)
    {
        m_used_surface_format.format = VK_FORMAT_UNDEFINED;
    }

    void render_context::initialize()
    {
        create_surface();
        select_present_queue();
        fetch_surface_capabilities();
        choose_surface_format();
        create_swap_chain();
        create_render_pass();
        create_frame_buffers();
    }

    render_context::~render_context()
    {
        MGE_DEBUG_TRACE(VULKAN) << "Destroy render context";
        m_swap_chain.reset();

        if (m_render_pass) {
            m_render_system.vkDestroyRenderPass(m_render_system.device(),
                                                m_render_pass,
                                                nullptr);
        }

        if (m_surface) {
            m_render_system.vkDestroySurfaceKHR(m_render_system.instance(),
                                                m_surface,
                                                nullptr);
        }
    }

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

        VkRenderPassCreateInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_info.attachmentCount = 1;
        render_pass_info.pAttachments = &color_attachment;
        render_pass_info.subpassCount = 1;
        render_pass_info.pSubpasses = &subpass;

        CHECK_VK_CALL(
            m_render_system.vkCreateRenderPass(m_render_system.device(),
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

    void render_context::create_swap_chain()
    {
        m_swap_chain = std::make_shared<mge::vulkan::swap_chain>(*this);
    }

    void render_context::create_frame_buffers()
    {
        auto swap_chain =
            dynamic_cast<mge::vulkan::swap_chain*>(m_swap_chain.get());
        swap_chain->create_frame_buffers(m_render_pass);
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
        shader_ref result;
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
            }
        }
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

} // namespace mge::vulkan
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "render_system.hpp"
#include "window.hpp"

MGE_USE_LOG(VULKAN);

namespace vulkan {

    template <typename F, typename C>
    void render_context::fill_enumeration(const F &function, C &container)
    {
        uint32_t count = 0;
        function(&count, nullptr);
        if (count) {
            container.resize(count);
            function(&count, container.data());
        }
    }

    render_context::render_context(window *win, render_system &system,
                                   const system_config &config)
        : mge::render_context(win), m_render_system(system),
          m_surface(VK_NULL_HANDLE)
    {
        MGE_DEBUG_LOG(VULKAN) << "Create render context";
        create_surface(win);
    }

    render_context::~render_context()
    {
        if (m_surface) {
            m_render_system.vkDestroySurfaceKHR(m_render_system.instance(),
                                                m_surface, nullptr);
        }
    }

    void render_context::create_surface(window *win)
    {
#ifdef MGE_OS_WINDOWS
        MGE_DEBUG_LOG(VULKAN) << "Create Vulkan surface";
        VkWin32SurfaceCreateInfoKHR create_info = {};
        create_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.hinstance = GetModuleHandle(nullptr);
        create_info.hwnd      = win->hwnd();
        create_info.flags     = 0;
        CHECK_VK_CALL(m_render_system.vkCreateWin32SurfaceKHR(
            m_render_system.instance(), &create_info, nullptr, &m_surface));

        m_render_system.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            m_render_system.physical_device(), m_surface,
            &m_surface_capabilties);

        fill_enumeration(
            [&](uint32_t *count, VkSurfaceFormatKHR *data) {
                m_render_system.vkGetPhysicalDeviceSurfaceFormatsKHR(
                    m_render_system.physical_device(), m_surface, count, data);
            },
            m_surface_formats);
        MGE_DEBUG_LOG(VULKAN)
            << "Surface supports " << m_surface_formats.size() << " formats";

        fill_enumeration(
            [&](uint32_t *count, VkPresentModeKHR *data) {
                m_render_system.vkGetPhysicalDeviceSurfacePresentModesKHR(
                    m_render_system.physical_device(), m_surface, count, data);
            },
            m_surface_present_modes);

        MGE_DEBUG_LOG(VULKAN)
            << "Surface supports " << m_surface_present_modes.size()
            << " present modes";

#else
#    error "Unsupported platform"
#endif
    }

    void render_context::flush() { MGE_THROW_NOT_IMPLEMENTED; }

    mge::vertex_buffer_ref
    render_context::create_vertex_buffer(const mge::vertex_layout &layout,
                                         mge::usage usage, size_t element_count,
                                         void *initial_data)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type, mge::usage usage,
                                        size_t element_count,
                                        void * initial_data)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d(const mge::image_ref &image)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref render_context::create_texture_2d()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::shader_program_ref
    render_context::create_shader_program(mge::shader_type type)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::pipeline_ref render_context::create_pipeline()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void render_context::execute(const mge::command_list_ref &commands)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void render_context::shader_languages(
        std::vector<mge::shader_language> &languages) const
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

} // namespace vulkan

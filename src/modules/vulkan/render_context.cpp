// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "render_system.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(render_system& system_, window& window_)
        : m_render_system(system_)
        , m_window(window_)
        , m_surface(VK_NULL_HANDLE)
    {}

    void render_context::initialize()
    {
        create_surface();
        create_swap_chain();
    }

    render_context::~render_context()
    {
        if (m_surface) {
            m_render_system.vkDestroySurfaceKHR(m_render_system.instance(),
                                                m_surface,
                                                nullptr);
        }
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

} // namespace mge::vulkan
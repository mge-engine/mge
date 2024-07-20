// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& render_system_, window& window_);
        ~render_context();

        mge::index_buffer_ref  create_index_buffer(data_type dt,
                                                   size_t    data_size,
                                                   void*     data) override;
        mge::vertex_buffer_ref create_vertex_buffer(const vertex_layout& layout,
                                                    size_t data_size,
                                                    void*  data) override;
        mge::shader_ref        create_shader(shader_type t) override;
        mge::program_ref       create_program() override;
        mge::command_list_ref  create_command_list() override;
        mge::texture_ref       create_texture(texture_type type) override;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define DEVICE_FUNCTION(X) PFN_##X X{nullptr};

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

    private:
        void create_surface();
        void create_device();
        void teardown();
        void resolve_device_functions();
        void clear_functions();

        render_system& m_render_system;
        window&        m_window;
        VkSurfaceKHR   m_surface{VK_NULL_HANDLE};
        VkDevice       m_device{VK_NULL_HANDLE};
    };
} // namespace mge::vulkan
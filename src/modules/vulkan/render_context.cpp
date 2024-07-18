// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "mge/core/trace.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    render_context::render_context(render_system& render_system_,
                                   window&        window_)
    {}

    render_context::~render_context() {}

    mge::index_buffer_ref render_context::create_index_buffer(data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref result;
        return result;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref result;
        return result;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref result;
        return result;
    }

    mge::program_ref render_context::create_program()
    {
        mge::program_ref result;
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        mge::command_list_ref result;
        return result;
    }

    mge::texture_ref render_context::create_texture(texture_type type)
    {
        mge::texture_ref result;
        return result;
    }

} // namespace mge::vulkan
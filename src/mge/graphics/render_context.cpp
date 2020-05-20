// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
namespace mge {
    render_context::render_context(mge::render_system *system,
                                   async_executor *    display_executor)
        : m_render_system(system), m_display_executor(display_executor)
    {}

    void render_context::await(const std::function<void()> &f)
    {
        m_display_executor->await(f);
    }

    void render_context::assign_thread() {}

    shader_program_ref
    render_context::create_shader_program(shader_type type,
                                          const char *asset_name)
    {
        auto asset  = m_render_system->locate_shader(type, asset_name);
        auto shader = create_shader_program(type);
        return shader;
    }

} // namespace mge

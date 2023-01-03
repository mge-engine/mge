// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/program.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    program::program(render_context& context)
        : context_object(context)
        , m_needs_link(true)
    {}

    program::~program() {}

    void program::set_shader(const shader_ref& s)
    {
        if (!s) {
            MGE_THROW_ARGUMENT_NOT_NULL(shader);
        }

        if (s->type() == shader_type::COMPUTE) {
            MGE_THROW(mge::illegal_argument)
                << "Shader type must not be shader_type::COMPUTE";
        }

        if (!s->initialized()) {
            MGE_THROW(mge::illegal_argument)
                << "Shader must be initialized before attaching to program";
        }

        on_set_shader(s);
        m_needs_link = true;
    }

    void program::link()
    {
        if (m_needs_link) {
            on_link();
            m_needs_link = false;
        }
    }

    const program::attribute_list& program::attributes() const
    {
        assert_linked();
        return m_attributes;
    }

    const program::uniform_list& program::uniforms() const
    {
        assert_linked();
        return m_uniforms;
    }

    const program::uniform_buffer_list& program::uniform_buffers() const
    {
        assert_linked();
        return m_uniform_buffers;
    }

    void program::assert_linked() const
    {
        if (m_needs_link) {
            MGE_THROW(illegal_state) << "Program not linked";
        }
    }

} // namespace mge

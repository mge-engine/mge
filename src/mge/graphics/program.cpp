// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/program.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    program::program(render_context& context)
        : context_object(context)
        , m_needs_link(true)
    {}

    program::~program() {}

    void program::set_shader(shader_handle shader)
    {
        mge::shader* s = shader.get();

        if (!s) {
            MGE_THROW_ARGUMENT_NOT_NULL(shader);
        }

        if (s->type() == shader_type::COMPUTE) {
            MGE_THROW(mge::illegal_argument)
                << "Shader type must not be shader_type::COMPUTE";
        }

        context().prepare_frame([this, s]() {
            if (!s->initialized()) {
                MGE_THROW(mge::illegal_argument)
                    << "Shader must be initialized before attaching to "
                       "program";
            }
            this->on_set_shader(s);
            this->m_needs_link = true;
        });
    }

    void program::link()
    {
        if (m_needs_link) {
            context().prepare_frame([this]() {
                MGE_DEBUG_TRACE(GRAPHICS, "Linking program {}", (void*)this);
                this->on_link();
                this->m_needs_link = false;
                set_ready(true);
            });
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

    std::ostream& operator<<(std::ostream& os, const mge::program::attribute& a)
    {
        os << "attribute{name=" << a.name << ",type=" << a.type
           << ",size=" << static_cast<uint32_t>(a.size) << "}";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const mge::program::uniform& u)
    {
        /* os << "uniform{name=" << u.name << ",type=" << u.type
           << ",size=" << static_cast<uint32_t>(u.size) << "}"; */
        return os;
    }

    std::ostream& operator<<(std::ostream&                       os,
                             const mge::program::uniform_buffer& ub)
    {
        return os;
    }

} // namespace mge

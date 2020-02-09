// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/pipeline.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    pipeline::pipeline(render_context& context)
        :context_object(context)
        ,m_needs_link(true)
    {}

    pipeline::~pipeline()
    {}

    void
    pipeline::set_shader_program(const shader_program_ref& s)
    {
        if(!s) {
            MGE_THROW_ARGUMENT_NOT_NULL(s);
        }

        if(s->type() == shader_type::COMPUTE) {
            MGE_THROW(mge::illegal_argument) << "Shader type must not be shader_type::COMPUTE";
        }

        if(!s->defined()) {
            MGE_THROW(mge::illegal_argument) << "Shader must be defined before attaching to pipeline";
        }

        on_set_shader_program(s);
        m_needs_link = true;
    }

    void
    pipeline::link()
    {
        if(m_needs_link) {
            on_link();
            m_needs_link = false;
        }
    }

}

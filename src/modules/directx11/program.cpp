// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/to_underlying.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge::dx11 {

    program::program(render_context& context)
        : ::mge::program(context)
    {}

    program::~program() {}

    void program::on_link()
    {
        if (!program_shader(mge::shader_type::VERTEX) ||
            !program_shader(mge::shader_type::FRAGMENT)) {
            MGE_THROW(mge::illegal_state) << "Cannot link empty program";
        }
    }

    void program::on_set_shader(const shader_ref& shader)
    {
        auto index = mge::to_underlying(shader->type());
        m_shaders[index] = shader;
    }

} // namespace mge::dx11
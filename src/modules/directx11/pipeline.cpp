// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "pipeline.hpp"
#include "mge/core/to_underlying.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    pipeline::pipeline(render_context& context)
        : ::mge::pipeline(context)
    {}

    pipeline::~pipeline() {}

    void pipeline::on_link() {}

    void pipeline::on_set_shader(const shader_ref& shader)
    {
        auto index = mge::to_underlying(shader->type());
        m_shaders[index] = shader;
    }

} // namespace mge::dx11
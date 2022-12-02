// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "pipeline.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    pipeline::pipeline(render_context& context)
        : ::mge::pipeline(context)
    {}

    pipeline::~pipeline() {}

    void pipeline::on_link() {}

    void pipeline::on_set_shader(const shader_ref& shader) {}

} // namespace mge::dx11
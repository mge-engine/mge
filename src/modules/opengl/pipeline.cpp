// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "pipeline.hpp"
#include "render_context.hpp"

namespace mge::opengl {
    pipeline::pipeline(render_context& context)
        : mge::pipeline(context)
    {}

    pipeline::~pipeline() {}
} // namespace mge::opengl
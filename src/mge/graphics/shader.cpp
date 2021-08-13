// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"

namespace mge {

    shader::shader(render_context& context, shader_type t)
        : context_object(context)
        , m_type(t)
        , m_initialized(false)
    {}

} // namespace mge
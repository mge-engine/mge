// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"

namespace mge {
    shader::shader(render_context& context, shader_type type)
        :context_object(context)
        ,m_type(type)
    {}

    shader::~shader()
    {}
}

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    shader::shader(render_context& context, shader_type t)
        : context_object(context)
        , m_type(t)
        , m_initialized(false)
    {}

    void shader::compile(std::string_view source)
    {
        on_compile(source);
        m_initialized = true;
    }

    void shader::set_code(const mge::buffer& code)
    {
        on_set_code(code);
        m_initialized = true;
    }

    shader_type shader::type() const
    {
        return m_type;
    }

    bool shader::initialized() const
    {
        return m_initialized;
    }

    void shader::destroy()
    {
        context().destroy_shader(this);
    }

} // namespace mge
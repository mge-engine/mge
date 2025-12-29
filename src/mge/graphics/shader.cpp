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
        m_initialized = false;
        auto src = std::make_shared<std::string>(source);
        context().prepare_frame([this, src]() {
            this->on_compile(*src);
            this->m_initialized = true;
        });
    }

    void shader::set_code(const mge::buffer& code)
    {
        m_initialized = false;
        auto c = std::make_shared<mge::buffer>(code);
        context().prepare_frame([this, c]() {
            this->on_set_code(*c);
            this->m_initialized = true;
        });
    }

    shader_type shader::type() const
    {
        return m_type;
    }

    bool shader::initialized() const
    {
        return m_initialized;
    }

} // namespace mge
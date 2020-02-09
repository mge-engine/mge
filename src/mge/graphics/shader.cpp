// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"

namespace mge {
    shader_program::shader_program(render_context& context, shader_type type)
        :context_object(context)
        ,m_type(type)
        ,m_defined(false)
    {}

    shader_program::~shader_program()
    {}

    void
    shader_program::compile(const std::string& source_code)
    {
        // compiling is destructive, old definition is not guaranteed
        // to prevail if compilation fails
        m_defined = false; 
        try {
            on_compile(source_code);
            m_defined = true;
        } catch(...) {
            mge::rethrow();
        }
    }

    void
    shader_program::load(const mge::buffer& buf)
    {
        // loading is destructive, old definition is not guaranteed
        // to prevail if compilation fails
        m_defined = false;
        try {
            on_load(buf);
            m_defined = true;
        } catch(...) {
            mge::rethrow();
        }
    }
}

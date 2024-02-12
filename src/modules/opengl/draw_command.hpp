// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/draw_command.hpp"
#include "mge/graphics/render_context.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class draw_command : public mge::draw_command
    {
    public:
        draw_command(render_context&          context,
                     const program_ref&       program,
                     const vertex_buffer_ref& vertices,
                     const index_buffer_ref&  indices,
                     mge::topology            t);
        virtual ~draw_command();

        void prepare() const;

        GLuint vao() const { return m_vao; }

    private:
        mutable GLuint m_vao;
    };
} // namespace mge::opengl

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/program.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context;

    class program : public mge::program
    {
    public:
        program(render_context& context);
        virtual ~program();

        void on_link() override;
        void on_set_shader(const shader_ref& shader) override;

        GLuint program_name() const noexcept { return m_program; }

    private:
        void dump_info_log();
        void collect_uniforms();

        GLuint m_program;
    };

    inline GLuint gl_program(const mge::program& p)
    {
        const opengl::program& ogl_p = static_cast<const opengl::program&>(p);
        return ogl_p.program_name();
    }

    inline GLuint gl_program(const mge::program_ref& p)
    {
        if (p) {
            return gl_program(*p);
        } else {
            return 0;
        }
    }

} // namespace mge::opengl
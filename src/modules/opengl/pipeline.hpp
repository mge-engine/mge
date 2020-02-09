// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/pipeline.hpp"
#include "opengl.hpp"

namespace opengl {
    class pipeline : public mge::pipeline
    {
    public:
        pipeline(mge::render_context& context);
        virtual ~pipeline();
        GLuint name() const { return m_program; }
    protected:
        void on_link();
        void on_set_shader_program(const mge::shader_program_ref& shader);
    private:
        void dump_info_log();
        void collect_attributes();
        void clear_vaos();

        GLuint m_program;
    };

    inline GLuint gl_program(const mge::pipeline& p)
    {
        const opengl::pipeline& ogl_p = static_cast<const opengl::pipeline&>(p);
        return ogl_p.name();
    }

    inline GLuint gl_program(const mge::pipeline_ref& p)
    {
        if (p) {
            return gl_program(*p);
        } else {
            return 0;
        }
    }
}

// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"
#include "opengl.hpp"

namespace opengl {

    class shader : mge::shader
    {
    public:
        shader(mge::render_context& context,
               mge::shader_type type);
        virtual ~shader();
        void compile(const std::string& source_code) override;
        void load(const mge::buffer& code) override;
    private:
        void create_shader();
        GLenum gl_shader_type() const;
        GLuint m_shader;
    };
}

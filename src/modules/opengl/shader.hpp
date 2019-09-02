// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"
#include "opengl.hpp"

namespace opengl {

    class shader : public mge::shader
    {
    public:
        shader(mge::render_context& context,
               mge::shader_type type);
        virtual ~shader();
        inline GLuint gl_shader() const noexcept { return m_shader; }
    protected:
        void on_compile(const std::string& source_code) override;
        void on_load(const mge::buffer& code) override;
    private:
        void create_shader();
        void throw_compilation_error();
        GLenum gl_shader_type() const;
        GLuint m_shader;
    };
}

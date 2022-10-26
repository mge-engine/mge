// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader.hpp"
#include "opengl.hpp"

namespace mge::opengl {
    class render_context;

    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();

        GLuint gl_shader() const noexcept { return m_shader; }

    protected:
        void on_compile(std::string_view source) override;
        void on_set_code(const mge::buffer& code) override;

    private:
        void create_shader();
        void throw_compilation_error();

        GLenum gl_shader_type() const;
        GLuint m_shader;
    };
} // namespace mge::opengl
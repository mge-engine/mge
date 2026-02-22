// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/program.hpp"
#include "opengl.hpp"
#include <map>
#include <string>
#include <vector>

namespace mge::opengl {

    class render_context;

    class program : public mge::program
    {
    public:
        program(render_context& context);
        virtual ~program();

        void on_link() override;
        void on_set_shader(mge::shader* shader) override;

        GLuint program_name() const noexcept
        {
            return m_program;
        }

        GLuint block_index(const std::string& name) const;

        struct sampler_info
        {
            std::string name;
            GLint       location;
        };

        const std::vector<sampler_info>& sampler_locations() const noexcept
        {
            return m_sampler_locations;
        }

    private:
        void dump_info_log();
        void collect_uniforms();
        void collect_uniform_buffers();
        void collect_attributes();

        GLuint                        m_program;
        std::map<std::string, GLuint> m_block_indices;
        std::vector<sampler_info>     m_sampler_locations;
    };

    inline GLuint gl_program(const mge::program& p)
    {
        const opengl::program& ogl_p = static_cast<const opengl::program&>(p);
        return ogl_p.program_name();
    }

    inline GLuint gl_program(mge::program* p)
    {
        if (p) {
            return gl_program(*p);
        } else {
            return 0;
        }
    }

} // namespace mge::opengl
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/graphics/command_list.hpp"
#include "opengl.hpp"
#include <variant>

namespace mge::opengl {
    class render_context;

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& ctx);
        ~command_list();

        void viewport(const mge::viewport& vp) override;
        void scissor(const mge::rectangle& rect) override;
        void default_scissor() override;
        void clear(const rgba_color& c) override;
        void clear_depth(float depth) override;
        void clear_stencil(int32_t stencil) override;
        void draw(const mge::draw_command& command) override;

        void execute() override;

    private:
        void destroy();

        struct clear_command
        {
            rgba_color clear_color;
        };

        struct clear_depth_command
        {
            float clear_depth;
        };

        struct clear_stencil_command
        {
            int32_t clear_stencil;
        };

        struct draw_command
        {
            GLuint  program_name;
            GLuint  vao;
            GLuint  topology;
            GLsizei element_count;
        };

        struct scissor_command
        {
            mge::rectangle scissor;
        };

        struct viewport_command
        {
            mge::viewport viewport;
        };

        using command = std::variant<std::monostate,
                                     clear_command,
                                     clear_depth_command,
                                     clear_stencil_command,
                                     draw_command,
                                     scissor_command,
                                     viewport_command>;

        using command_vector = std::vector<command>;

        render_context&              m_opengl_context;
        command_vector               m_commands;
        mge::small_vector<GLuint, 3> m_all_vaos;
    };
} // namespace mge::opengl

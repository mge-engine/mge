// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/graphics/command_list.hpp"
#include "opengl.hpp"
#include <variant>

namespace mge::opengl {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& ctx)
            : mge::command_list(ctx, false)
        {}

        ~command_list();

        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;

        void execute() override;

    private:
        void destroy();

        struct clear_command
        {
            rgba_color clear_color;
        };

        struct draw_command
        {
            GLuint  program_name;
            GLuint  vao;
            GLuint  topology;
            GLsizei element_count;
        };

        using command =
            std::variant<std::monostate, clear_command, draw_command>;

        using command_vector = std::vector<command>;

        command_vector               m_commands;
        mge::small_vector<GLuint, 3> m_all_vaos;
    };
} // namespace mge::opengl

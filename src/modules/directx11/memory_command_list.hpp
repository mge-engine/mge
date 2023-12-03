// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include "render_context.hpp"

namespace mge::dx11 {
    class memory_command_list : public mge::command_list
    {
    public:
        memory_command_list(render_context& context);
        ~memory_command_list();

        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;

    private:
        void perform_drawing(const mge::draw_command& command);

        struct clear_command
        {
            rgba_color clear_color;
        };

        struct draw_command
        {
            std::reference_wrapper<const mge::draw_command> command;
        };

        using command =
            std::variant<std::monostate, clear_command, draw_command>;

        using command_vector = std::vector<command>;

        render_context& m_dx11_context;
        command_vector  m_commands;
    };

} // namespace mge::dx11
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/dllexport.hpp"

#include <functional>
#include <variant>

namespace mge {
    class memory_command_list : public command_list
    {
    public:
        memory_command_list(render_context& ctx);
        ~memory_command_list() = default;

        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;

        void execute() override;

    private:
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

        command_vector m_commands;
    };
} // namespace mge
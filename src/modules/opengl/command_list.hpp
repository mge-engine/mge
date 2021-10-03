// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include <variant>
#include <vector>

namespace mge::opengl {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& ctx)
            : mge::command_list(ctx, false)
        {}

        ~command_list() = default;

        void clear(const rgba_color& c) override;
        void execute() override;

    private:
        struct clear_command
        {
            rgba_color clear_color;
        };

        using command = std::variant<std::monostate, clear_command>;
        using command_vector = std::vector<command>;

        command_vector m_commands;
    };
} // namespace mge::opengl

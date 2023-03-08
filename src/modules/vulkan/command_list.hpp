// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
namespace mge::vulkan {
    class render_context;

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void clear(const mge::rgba_color& color) override;
        void draw(const mge::draw_command& command) override;
        void finish() override;
        void execute() override;
    };
} // namespace mge::vulkan
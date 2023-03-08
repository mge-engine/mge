// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "render_context.hpp"

namespace mge::vulkan {
    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
    {}

    command_list::~command_list() {}

    void command_list::clear(const mge::rgba_color& color) {}

    void command_list::draw(const mge::draw_command& command) {}

    void command_list::finish() {}

    void command_list::execute() {}

} // namespace mge::vulkan
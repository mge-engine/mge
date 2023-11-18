// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_command_list.hpp"
#include "mge/core/overloaded.hpp"

namespace mge {
    memory_command_list::memory_command_list(render_context& ctx)
        : command_list(ctx, false)
    {}

    void memory_command_list::clear(const rgba_color& c)
    {
        m_commands.emplace_back(clear_command{c});
    }

    void memory_command_list::draw(const mge::draw_command& command)
    {
        m_commands.emplace_back(draw_command{command});
    }

    void memory_command_list::execute()
    {
        for (auto& c : m_commands) {
            std::visit(overloaded{[](std::monostate&) {},
                                  [](clear_command& c) {},
                                  [](draw_command& c) {}},
                       c);
        }
    }

} // namespace mge
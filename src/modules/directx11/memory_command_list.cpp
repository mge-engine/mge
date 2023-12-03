// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "memory_command_list.hpp"
#include "mge/core/overloaded.hpp"

namespace mge::dx11 {

    memory_command_list::memory_command_list(render_context& context)
        : mge::command_list(context, false)
        , m_dx11_context(context)
    {}

    memory_command_list::~memory_command_list() {}

    void memory_command_list::clear(const rgba_color& c)
    {
        m_commands.push_back(clear_command{c});
    }

    void memory_command_list::draw(const mge::draw_command& command)
    {
        m_commands.push_back(draw_command{command});
    }

    void memory_command_list::execute()
    {
        for (auto& cmd : m_commands) {
            std::visit(
                [&](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, clear_command>) {
                        float clear_color[4] = {arg.clear_color.r,
                                                arg.clear_color.g,
                                                arg.clear_color.b,
                                                arg.clear_color.a};
                        m_dx11_context.device_context()->ClearRenderTargetView(
                            m_dx11_context.render_target_view(),
                            clear_color);
                    } else if constexpr (std::is_same_v<T, draw_command>) {
                        // draw(arg.command.get());
                    }
                },
                cmd);
        }
    }

} // namespace mge::dx11
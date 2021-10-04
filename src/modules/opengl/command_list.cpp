// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "mge/core/overloaded.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    void command_list::clear(const rgba_color& c)
    {
        m_commands.emplace_back(clear_command{c});
    }

    void command_list::execute()
    {
        for (const auto& cmd : m_commands) {
            std::visit(overloaded{
                           [](const std::monostate&) {},
                           [](const clear_command& c) {
                               glClearColor(c.clear_color.r,
                                            c.clear_color.g,
                                            c.clear_color.b,
                                            c.clear_color.a);
                               CHECK_OPENGL_ERROR(glClearColor);
                               glClear(GL_COLOR_BUFFER_BIT);
                               CHECK_OPENGL_ERROR(glClear(GL_COLOR_BUFFER_BIT));
                           },
                       },
                       cmd);
        }
    }

} // namespace mge::opengl
// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"

namespace mge::opengl {

    void command_list::clear(const rgba_color& c)
    {
        clear_command cmd{c};
        m_commands.emplace_back(clear_command{c});
    }

    void command_list::execute() {}

} // namespace mge::opengl
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_command_list.hpp"
namespace mge {

    memory_command_list::memory_command_list(render_context &context)
        :command_list(context, false)
    {}

}

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    command_list::command_list(render_context& context, bool native)
        : context_object(context)
        , m_native(native)
    {}

    command_list::~command_list() {}

    bool command_list::native() const
    {
        return m_native;
    }

    void command_list::finish() {}

    void command_list::destroy()
    {
        context().destroy_command_list(this);
    }

} // namespace mge
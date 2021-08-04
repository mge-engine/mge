// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_list.hpp"

namespace mge {

    command_list::command_list(render_context& context, bool native)
        : context_object(context)
        , m_native(native)
    {}

    command_list::~command_list() {}

    bool command_list::native() const { return m_native; }

} // namespace mge
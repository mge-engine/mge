// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/command_list.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {
    command_list::command_list(render_context &context, bool native)
        :context_object(context)
        ,m_native(native)
        ,m_finished(false)
    {}

    command_list::~command_list()
    {}

    void
    command_list::finish()
    {
        if(!m_finished) {
            on_finish();
            m_finished = true;
        }
    }

    void
    command_list::assert_not_immutable()
    {
        if (immutable()) {
            MGE_THROW(mge::illegal_state) << "Command list is finished and immutable";
        }
    }


}

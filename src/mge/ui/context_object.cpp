// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/context_object.hpp"
#include "mge/ui/context.hpp"

namespace mge {
    namespace ui {
        context_object::context_object(::mge::ui::context &ctx)
            : m_context(ctx.shared_from_this())
        {}

        ::mge::ui::context &context_object::context() { return *m_context; }

        const mge::ui::context &context_object::context() const
        {
            return *m_context;
        }

    } // namespace ui
} // namespace mge

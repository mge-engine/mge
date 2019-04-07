// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge {

    context_object::context_object(render_context& context)
    {
        m_context = context.shared_from_this();
    }

    context_object::~context_object()
    {}

    render_context_ref
    context_object::context() const
    {
        return m_context;
    }

    void
    context_object::assert_same_context(const render_context &context) const
    {
        if(&context != m_context.get()) {
            MGE_THROW(mge::illegal_state) << "Different render context";
        }
    }

    void
    context_object::await(const std::function<void ()> &f)
    {
        if(m_context) {
            m_context->await(f);
        } else {
            f();
        }
    }

}

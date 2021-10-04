// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/context_object.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    context_object::context_object(render_context& context)
    {
        // trick: ensure there is at least one holder of
        // context, to initialize weak reference properly
        // in case the object is created in a render context's
        // constructor (or constructor of derived class)
        auto holder =
            std::shared_ptr<render_context>(&context, [](render_context*) {});
        m_context = context.shared_from_this();
    }

    context_object::~context_object() {}

    render_context& context_object::context() { return *m_context; }

    const render_context& context_object::context() const { return *m_context; }

    void context_object::assert_same_context(const render_context& c) const
    {
        if (m_context.get() != &c) {
            MGE_THROW(illegal_state) << "Render contexts differ";
        }
    }

    void context_object::assert_same_context(const context_object& cobj) const
    {
        if (m_context.get() != &cobj.context()) {
            MGE_THROW(illegal_state) << "Render contexts differ";
        }
    }

} // namespace mge
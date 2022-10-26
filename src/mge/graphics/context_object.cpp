// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/context_object.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    context_object::context_object(render_context& context)
    {
        try {
            m_context = context.shared_from_this();
        } catch (const std::bad_weak_ptr& e) {
            MGE_THROW(mge::illegal_state)
                << "Cannot create context object from incomplete context "
                   "reference: "
                << e.what();
        }
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

    void context_object::assert_same_context(
        const std::shared_ptr<context_object>& cobj) const
    {
        if (!cobj) {
            MGE_THROW(null_pointer) << "Cannot check null reference";
        }
        assert_same_context(*cobj);
    }

} // namespace mge
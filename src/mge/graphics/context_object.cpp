// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/context_object.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    context_object::context_object(render_context& context)
        : m_context(context)
    {}

    context_object::~context_object() {}

    render_context& context_object::context()
    {
        return m_context;
    }

    const render_context& context_object::context() const
    {
        return m_context;
    }

    void context_object::assert_same_context(const render_context& c) const
    {
        if (&m_context != &c) {
            MGE_THROW(illegal_state) << "Render contexts differ";
        }
    }

    void context_object::assert_same_context(const context_object& cobj) const
    {
        if (&m_context != &cobj.context()) {
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
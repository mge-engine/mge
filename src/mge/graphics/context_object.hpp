// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

#include <functional>

namespace mge {

    /**
     * An object that is valid in the context of a @c render_context like
     * a shader program, a texture, or a command list.
     */
    class MGE_GRAPHICS_EXPORT context_object
    {
    protected:
        explicit context_object(render_context& context);
    public:
        virtual ~context_object();
        render_context_ref context() const;
        void assert_same_context(const render_context& context) const;
        void await(const std::function<void()>& f);
    protected:
        render_context_ref m_context;
    };
}

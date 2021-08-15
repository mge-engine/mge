// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    /**
     * An object that is valid in the context of a @c render_context like
     * a shader program, a texture, or a command list.
     */
    class MGEGRAPHICS_EXPORT context_object : public noncopyable
    {
    protected:
        explicit context_object(render_context& context);

    public:
        virtual ~context_object();
        const render_context& context() const;
        render_context&       context();
        void assert_same_context(const render_context& c) const;
        void assert_same_context(const context_object& cobj) const;

    protected:
        render_context_ref m_context;
    };

} // namespace mge
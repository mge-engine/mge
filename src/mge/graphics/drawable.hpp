// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/types.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {

    /**
     * A drawable object.  A drawable object is some high level object that can
     * be presented on a render context.
     */
    class MGE_GRAPHICS_EXPORT drawable : public noncopyable
    {
    public:
        /**
         * Destructor.
         */
        virtual ~drawable() = default;

        /**
         * Retrieve whether the drawable has been prepared.
         * @return @c true if prepared
         */
        bool prepared() const noexcept { return m_prepared; }

        /**
         * Prepare the drawable.
         * @param context
         */
        void prepare(render_context &context);

        /**
         * @brief draw
         * @param context
         */
        void draw(render_context &context);

    protected:
        /**
         * Set prepared status.
         * @param prepared new status
         */
        void set_prepared(bool prepared) noexcept;

        virtual void on_prepare(render_context &context);
        virtual void on_draw(render_context &context) = 0;

        drawable();

        bool m_prepared;
    };
} // namespace mge

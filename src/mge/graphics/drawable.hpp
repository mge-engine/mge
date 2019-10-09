#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/core/types.hpp"
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
        virtual ~drawable();

        virtual bool prepared() const;
        virtual void prepare(render_context& context);
        virtual void draw(render_context& context) = 0;
    protected:
        drawable();
    };
}
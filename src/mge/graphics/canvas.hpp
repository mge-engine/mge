// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    /**
     * @brief A canvas is a drawing surface for 2d graphics.
     *
     * A canvas is a context object that can be used to draw
     * 2D graphics.
     *
     */
    class MGEGRAPHICS_EXPORT canvas : public context_object
    {
    protected:
        canvas(render_context& context);

    public:
        virtual ~canvas();
    };

} // namespace mge

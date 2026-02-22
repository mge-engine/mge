// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"
#include "mge/graphics/dllexport.hpp"

namespace mge {

    class MGEGRAPHICS_EXPORT frame_buffer : public mge::context_object
    {
    protected:
        frame_buffer(render_context& context);

    public:
        virtual ~frame_buffer() = default;
    };

} // namespace mge
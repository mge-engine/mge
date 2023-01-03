// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/context_object.hpp"

namespace mge {

    class MGEGRAPHICS_EXPORT texture : public context_object
    {
    protected:
        texture(render_context&);

    public:
        virtual ~texture();
    };

} // namespace mge
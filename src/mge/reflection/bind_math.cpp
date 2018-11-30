// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec2.hpp"
#include "mge/reflection/type.hpp"

namespace mge {
    namespace reflection {
        void bind_math()
        {
            type<mge::vec2f>()
                .constructor()
                .constructor<float, float>()
                .constructor<const vec2f&>()
                .destructor()
                .field("x", &vec2f::x)
                .field("y", &vec2f::y);
        }
    }
}

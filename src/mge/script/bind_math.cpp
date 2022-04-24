// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec2.hpp"

#include "mge/script/bind.hpp"
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    void bind_math()
    {
        module("mge")(type<mge::fvec2>("fvec2")
                          .field("x", &mge::fvec2::x)
                          .field("y", &mge::fvec2::y));
    }
} // namespace mge::script
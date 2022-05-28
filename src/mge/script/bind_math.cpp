// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec2.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    class math_script_binder : public script_binder
    {
    public:
        math_script_binder() = default;

        void bind()
        {
            module("mge")(type<mge::fvec2>("fvec2")
                              .field("x", &mge::fvec2::x)
                              .field("y", &mge::fvec2::y));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(math_script_binder,
                                mge::script_binder,
                                mgemath,
                                math);

} // namespace mge::script
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/math/vec2.hpp"
#include "mge/math/vec3.hpp"
#include "mge/math/vec4.hpp"

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
                              .constructor()
                              .copy_constructor()
                              .constructor<float, float>()
                              .field("x", &mge::fvec2::x)
                              .field("y", &mge::fvec2::y));
            module("mge")(type<mge::fvec3>("fvec3")
                              .constructor()
                              .copy_constructor()
                              .constructor<float, float, float>()
                              .field("x", &mge::fvec3::x)
                              .field("y", &mge::fvec3::y)
                              .field("z", &mge::fvec3::z));
            module("mge")(type<mge::fvec4>("fvec4")
                              .constructor()
                              .copy_constructor()
                              .constructor<float, float, float, float>()
                              .field("x", &mge::fvec4::x)
                              .field("y", &mge::fvec4::y)
                              .field("z", &mge::fvec4::z)
                              .field("w", &mge::fvec4::w));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(math_script_binder,
                                mge::script_binder,
                                mgemath,
                                math);

} // namespace mge::script
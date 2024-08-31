// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

#include "mge/math/vec2.hpp"

namespace mge::script {
    class math_script_binder : public script_binder
    {
    public:
        math_script_binder() = default;

        void bind() override
        {
            mge::script::module mge("mge");
            mge(type<mge::fvec2>("fvec2"));
        }
    };

    MGE_REGISTER_IMPLEMENTATION(math_script_binder,
                                mge::script_binder,
                                mgemath,
                                math);

} // namespace mge::script
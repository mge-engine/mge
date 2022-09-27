// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class script_script_binder : public script_binder
    {
    public:
        script_script_binder() = default;
        void bind() { module("mge")(module("script")); }
    };

    MGE_REGISTER_IMPLEMENTATION(script_script_binder,
                                mge::script_binder,
                                mgescript,
                                script);

} // namespace mge::script
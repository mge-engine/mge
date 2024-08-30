// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    class graphics_script_binder : public script_binder
    {
    public:
        graphics_script_binder() = default;

        void bind() override { mge::script::module mge("mge"); }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_script_binder,
                                mge::script_binder,
                                mgegraphics graphics);

} // namespace mge::script
// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/window.hpp"

#include "mge/script/bind.hpp"
#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/type.hpp"

namespace mge::script {
    void bind_graphics()
    {
        module("mge")(
            type<mge::shader_type>("shader_type")
                .enum_value("VERTEX", mge::shader_type::VERTEX)
                .enum_value("FRAGMENT", mge::shader_type::FRAGMENT)
                .enum_value("COMPUTE", mge::shader_type::COMPUTE)
                .enum_value("TESSELATION_CONTROL",
                            mge::shader_type::TESSELATION_CONTROL)
                .enum_value("TESSELATION_EVALUATION",
                            mge::shader_type::TESSELATION_EVALUATION)
                .enum_value("GEOMETRY", mge::shader_type::GEOMETRY),
            type<mge::window_options>("window_options")(
                type<mge::window_options::option>("option")),
            type<mge::render_system>("render_system")
                .method("create_window",
                        static_cast<mge::window_ref (render_system::*)(void)>(
                            &mge::render_system::create_window)),
            type<mge::window>("window"));
    }
} // namespace mge::script
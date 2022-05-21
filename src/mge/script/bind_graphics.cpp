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
                type<mge::window_options::option>("option")
                    .enum_value("NONE", mge::window_options::NONE)
                    .enum_value("FULLSCREEN", mge::window_options::FULLSCREEN)
                    .enum_value("FULLSCREEN_WINDOWED",
                                mge::window_options::FULLSCREEN_WINDOWED)
                    .enum_value("RESIZABLE", mge::window_options::RESIZABLE)
                    .enum_value("MAXIMIZED", mge::window_options::MAXIMIZED)
                    .enum_value("MINIMIZED", mge::window_options::MINIMIZED)
                    .enum_value("DECORATED", mge::window_options::DECORATED)
                    .enum_value("TITLE", mge::window_options::TITLE)
                    .enum_value("MAX", mge::window_options::MAX))
                .constructor()
                .copy_constructor()
                .method("set_option", &mge::window_options::set_option)
                .method("option_set", &mge::window_options::option_set)
                .method("standard_options",
                        &mge::window_options::standard_options)
                .method("fullscreen_options",
                        &mge::window_options::fullscreen_options),
            type<mge::extent>("extent")
                .constructor()
                .constructor<uint32_t, uint32_t>()
                .copy_constructor()
                .field("width", &mge::extent::width)
                .field("height", &mge::extent::height),
            type<mge::render_system>("render_system")(
                type<render_system::monitor_collection>("monitor_collection"))
                .method("create_window",
                        static_cast<mge::window_ref (render_system::*)(void)>(
                            &mge::render_system::create_window))
                .method("create_window",
                        static_cast<mge::window_ref (render_system::*)(
                            const mge::extent&,
                            const mge::window_options&)>(
                            &mge::render_system::create_window))
                .method("create",
                        static_cast<mge::render_system_ref (*)()>(
                            &mge::render_system::create))
                .method(
                    "create",
                    static_cast<mge::render_system_ref (*)(std::string_view)>(
                        &mge::render_system::create)),
            type<mge::window>("window"));
    }
} // namespace mge::script
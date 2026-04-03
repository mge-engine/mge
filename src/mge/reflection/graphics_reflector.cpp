// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/graphics/data_type.hpp"
#include "mge/graphics/pass.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/vertex_format.hpp"
#include "mge/graphics/window.hpp"
#include "mge/input/input_handler.hpp"

namespace mge::reflection {

    class graphics_reflector : public reflector
    {
    public:
        graphics_reflector() = default;
        ~graphics_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::data_type>());
            mge(type<mge::topology>());
            mge(type<mge::point>()
                    .constructor<uint32_t, uint32_t>()
                    .field("x", &mge::point::x)
                    .field("y", &mge::point::y));
            mge(type<mge::vertex_format>()
                    .constructor<mge::data_type, size_t>()
                    .method("binary_size", &mge::vertex_format::binary_size));
            mge(type<mge::render_system>()
                    .static_method("create",
                                   static_cast<mge::render_system_ref (*)()>(
                                       &mge::render_system::create))
                    .static_method(
                        "create_with",
                        static_cast<mge::render_system_ref (*)(
                            std::string_view)>(&mge::render_system::create))
                    .method(
                        "create_window",
                        static_cast<mge::window_ref (mge::render_system::*)()>(
                            &mge::render_system::create_window))
                    .method("system_capabilities",
                            &mge::render_system::system_capabilities)
                    .method("monitors", &mge::render_system::monitors)
                    .method("frame_debugger",
                            &mge::render_system::frame_debugger));
            mge(type<mge::rgba_color>()
                    .constructor<float, float, float, float>()
                    .field("r", &mge::rgba_color::r)
                    .field("g", &mge::rgba_color::g)
                    .field("b", &mge::rgba_color::b)
                    .field("a", &mge::rgba_color::a));
            mge(type<mge::window>()
                    .method("show", &mge::window::show)
                    .method("hide", &mge::window::hide)
                    .method(
                        "render_context",
                        static_cast<mge::render_context& (mge::window::*)()>(
                            &mge::window::render_context))
                    .method("set_close_listener",
                            &mge::window::set_close_listener)
                    .method("add_key_action_handler",
                            static_cast<mge::input_handler::key_type (
                                mge::window::*)(
                                const mge::input_handler::key_action_handler&)>(
                                &mge::input_handler::add_key_action_handler)));
            mge(type<mge::render_context>()
                    .method("pass", &mge::render_context::pass)
                    .method("frame", &mge::render_context::frame));
            mge(type<mge::pass>()
                    .method("default_viewport", &mge::pass::default_viewport)
                    .method("clear_color", &mge::pass::clear_color)
                    .method("touch", &mge::pass::touch));
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 4> deps{
                {"core", "std", "math", "input"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_reflector,
                                mge::reflection::reflector,
                                graphics);
} // namespace mge::reflection

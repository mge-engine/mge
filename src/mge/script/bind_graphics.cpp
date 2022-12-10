// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/buffer_type.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/hardware_buffer.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/monitor.hpp"
#include "mge/graphics/point.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/rgb_color.hpp"
#include "mge/graphics/rgba_color.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/graphics/shader_language.hpp"
#include "mge/graphics/shader_type.hpp"
#include "mge/graphics/swap_chain.hpp"
#include "mge/graphics/topology.hpp"
#include "mge/graphics/vertex_buffer.hpp"
#include "mge/graphics/vertex_layout.hpp"
#include "mge/graphics/window.hpp"

#include "mge/script/function.hpp"
#include "mge/script/module.hpp"
#include "mge/script/script_binder.hpp"
#include "mge/script/type.hpp"

namespace mge::script {

    class graphics_script_binder : public script_binder
    {
    public:
        graphics_script_binder() = default;

        void bind()
        {
            module("mge")(
                type<mge::render_context>("render_context")
                    .method("create_index_buffer",
                            &mge::render_context::create_index_buffer)
                    .method("create_vertex_buffer",
                            &mge::render_context::create_vertex_buffer)
                    .method("create_shader",
                            &mge::render_context::create_shader)
                    .method("create_command_list",
                            &mge::render_context::create_command_list)
                    .method("swap_chain", &mge::render_context::swap_chain),
                type<mge::buffer_type>("buffer_type")
                    .enum_value("VERTEX", mge::buffer_type::VERTEX)
                    .enum_value("INDEX", mge::buffer_type::INDEX)
                    .enum_value("CONSTANT", mge::buffer_type::CONSTANT),
                type<mge::command_list>("command_list")
                    .method("native", &mge::command_list::native)
                    .method("clear", &mge::command_list::clear)
                    .method("execute", &mge::command_list::execute),
                type<mge::index_buffer>("index_buffer")
                    .base(type<mge::hardware_buffer>())
                    .method("element_type", &mge::index_buffer::element_type)
                    .method("element_count", &mge::index_buffer::element_count),
                type<mge::vertex_buffer>("vertex_buffer")
                    .base(type<mge::hardware_buffer>())
                    .method("layout", &vertex_buffer::layout)
                    .method("element_count",
                            &mge::vertex_buffer::element_count),
                type<mge::vertex_layout>("vertex_layout")
                    .constructor()
                    .copy_constructor()
                    .method("binary_size", &vertex_layout::binary_size),
                type<mge::vertex_format>("vertex_format")
                    .constructor()
                    .constructor<data_type, size_t>()
                    .copy_constructor()
                    .method("type", &vertex_format::type)
                    .method("size", &vertex_format::size)
                    .method("binary_size", &vertex_format::binary_size),
                type<mge::hardware_buffer>("hardware_buffer")
                    .method("size", &mge::hardware_buffer::size)
                    .method("map", &mge::hardware_buffer::map)
                    .method("unmap", &mge::hardware_buffer::unmap)
                    .method("mapped", &mge::hardware_buffer::mapped)
                    .method("type", &mge::hardware_buffer::type),
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
                        .enum_value("FULLSCREEN",
                                    mge::window_options::FULLSCREEN)
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
                type<mge::render_system>("render_system")(
                    type<render_system::monitor_collection>(
                        "monitor_collection"))
                    .method(
                        "create_window",
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
                    .method("create",
                            static_cast<mge::render_system_ref (*)(
                                std::string_view)>(&mge::render_system::create))
                    .method("monitors", &mge::render_system::monitors),
                type<monitor>("monitor")
                    .method("name", &mge::monitor::name)
                    .method("primary", &mge::monitor::primary)
                    .method("physical_size", &mge::monitor::physical_size)
                    .method("current_mode", &mge::monitor::current_mode)
                    .method("supported_modes", &mge::monitor::current_mode),
                type<pipeline>("pipeline")(
                    type<pipeline::attribute_list>("attribute_list"),
                    type<pipeline::uniform_buffer_list>("uniform_buffer_list"),
                    type<pipeline::uniform_buffer>("uniform_buffer")
                        .field("name", &mge::pipeline::uniform_buffer::name)
                    /* .field("uniforms",
                       &mge::pipeline::uniform_buffer::uniforms) */
                    ,
                    type<pipeline::uniform_list>("uniform_list"),
                    type<pipeline::attribute>("attribute")
                        .field("name", &mge::pipeline::attribute::name)
                        .field("type", &mge::pipeline::attribute::type)
                        .field("size", &mge::pipeline::attribute::size),
                    type<pipeline::uniform>("uniform")
                        .field("name", &mge::pipeline::uniform::name)
                        .field("type", &mge::pipeline::uniform::type)
                        .field("size", &mge::pipeline::uniform::size))
                    .method("set_shader", &mge::pipeline::set_shader)
                    .method("link", &mge::pipeline::link)
                    .method("needs_link", &mge::pipeline::needs_link)
                    .method("attributes", &mge::pipeline::attributes)
                    .method("uniforms", &mge::pipeline::uniforms)
                    .method("uniform_buffers", &mge::pipeline::uniform_buffers),
                type<mge::window>("window"),
                type<mge::shader>("shader")
                    .method("compile", &shader::compile)
                    .method("set_code", &shader::set_code)
                    .method("type", &shader::type)
                    .method("initialized", &shader::initialized),
                type<mge::swap_chain>("swap_chain")
                    .method("present", &swap_chain::present),
                type<rgb_color>("rgb_color")
                    .constructor()
                    .constructor<unsigned int>()
                    .constructor<const char*>()
                    .constructor<float>()
                    .constructor<float, float, float>()
                    .copy_constructor()
                    .field("r", &mge::rgb_color::r)
                    .field("g", &mge::rgb_color::g)
                    .field("b", &mge::rgb_color::b),
                type<rgba_color>("rgba_color")
                    .constructor()
                    .constructor<unsigned int>()
                    .constructor<const char*>()
                    .constructor<const char*, float>()
                    .constructor<float, float>()
                    .constructor<float, float, float, float>()
                    .copy_constructor()
                    .field("r", &mge::rgba_color::r)
                    .field("g", &mge::rgba_color::g)
                    .field("b", &mge::rgba_color::b)
                    .field("a", &mge::rgba_color::a),
                type<shader_language>("shader_language")
                    .constructor()
                    .constructor<std::string_view, const semantic_version&>()
                    .method("name", &shader_language::name)
                    .method("version", &shader_language::version)
                    .method("compatible", &shader_language::compatible),
                type<point>("point")
                    .constructor()
                    .constructor<uint32_t, uint32_t>()
                    .copy_constructor()
                    .field("x", &point::x)
                    .field("y", &point::y),
                type<mge::topology>("topology")
                    .enum_value("TRIANGLES", mge::topology::TRIANGLES)
                    .enum_value("TRIANGLE_STRIP", mge::topology::TRIANGLE_STRIP)
                    .enum_value("LINES", mge::topology::LINES)
                    .enum_value("LINE_STRIP", mge::topology::LINE_STRIP),
                type<mge::data_type>("data_type")
                    .enum_value("UNKNOWN", mge::data_type::UNKNOWN)
                    .enum_value("INT8", mge::data_type::INT8)
                    .enum_value("UINT8", mge::data_type::UINT8)
                    .enum_value("INT16", mge::data_type::INT16)
                    .enum_value("UINT16", mge::data_type::UINT16)
                    .enum_value("INT32", mge::data_type::INT32)
                    .enum_value("UINT32", mge::data_type::UINT32)
                    .enum_value("INT64", mge::data_type::INT64)
                    .enum_value("UINT64", mge::data_type::UINT64)
                    .enum_value("INT128", mge::data_type::INT128)
                    .enum_value("UINT128", mge::data_type::UINT128)
                    .enum_value("HALF", mge::data_type::HALF)
                    .enum_value("FLOAT", mge::data_type::FLOAT)
                    .enum_value("DOUBLE", mge::data_type::DOUBLE)
                    .enum_value("LONG_DOUBLE", mge::data_type::LONG_DOUBLE),
                type<mge::extent>("extent")
                    .constructor()
                    .constructor<uint32_t, uint32_t>()
                    .copy_constructor()
                    .field("width", &mge::extent::width)
                    .field("height", &mge::extent::height)
                // module end
            );
        }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_script_binder,
                                mge::script_binder,
                                mgegraphics,
                                graphics);

} // namespace mge::script
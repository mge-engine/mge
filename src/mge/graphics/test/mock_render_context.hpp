// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "test/googlemock.hpp"

class MOCK_render_context : public mge::render_context
{
public:
    MOCK_render_context()
        : mge::render_context(mge::extent(800, 600))
    {}
    ~MOCK_render_context() {}

    MOCK_METHOD(mge::index_buffer*,
                create_index_buffer,
                (mge::data_type, size_t, void*),
                ());
    MOCK_METHOD(void, destroy_index_buffer, (mge::index_buffer * ib), ());
    MOCK_METHOD(mge::vertex_buffer*,
                create_vertex_buffer,
                (const mge::vertex_layout&, size_t, void*),
                ());
    MOCK_METHOD(void, destroy_vertex_buffer, (mge::vertex_buffer * vb), ());
    MOCK_METHOD(mge::shader*, on_create_shader, (mge::shader_type), ());
    MOCK_METHOD(void, on_destroy_shader, (mge::shader * s), ());
    MOCK_METHOD(mge::program*, on_create_program, (), ());
    MOCK_METHOD(void, on_destroy_program, (mge::program * p), ());

    MOCK_METHOD(mge::command_list*, create_command_list, (), ());
    MOCK_METHOD(void, destroy_command_list, (mge::command_list * cl), ());
    MOCK_METHOD(mge::frame_command_list*,
                create_current_frame_command_list,
                (),
                ());
    MOCK_METHOD(void,
                destroy_frame_command_list,
                (mge::frame_command_list * fcl),
                ());
    MOCK_METHOD(mge::texture_ref, create_texture, (mge::texture_type), ());
};
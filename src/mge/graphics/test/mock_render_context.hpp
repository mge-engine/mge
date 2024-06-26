// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "test/googlemock.hpp"

class MOCK_render_context : public mge::render_context
{
public:
    MOCK_render_context() {}
    ~MOCK_render_context() {}

    MOCK_METHOD(mge::index_buffer_ref,
                create_index_buffer,
                (mge::data_type, size_t, void*),
                ());
    MOCK_METHOD(mge::vertex_buffer_ref,
                create_vertex_buffer,
                (const mge::vertex_layout&, size_t, void*),
                ());
    MOCK_METHOD(mge::shader_ref, create_shader, (mge::shader_type), ());
    MOCK_METHOD(mge::program_ref, create_program, (), ());
    MOCK_METHOD(mge::command_list_ref, create_command_list, (), ());
    MOCK_METHOD(mge::texture_ref, create_texture, (mge::texture_type), ());
};
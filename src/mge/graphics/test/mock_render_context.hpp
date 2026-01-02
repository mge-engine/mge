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
                on_create_index_buffer,
                (mge::data_type, size_t),
                ());
    MOCK_METHOD(void, on_destroy_index_buffer, (mge::index_buffer * ib), ());
    MOCK_METHOD(mge::vertex_buffer*,
                on_create_vertex_buffer,
                (const mge::vertex_layout&, size_t),
                ());
    MOCK_METHOD(void, on_destroy_vertex_buffer, (mge::vertex_buffer * vb), ());
    MOCK_METHOD(mge::shader*, on_create_shader, (mge::shader_type), ());
    MOCK_METHOD(void, on_destroy_shader, (mge::shader * s), ());
    MOCK_METHOD(mge::program*, on_create_program, (), ());
    MOCK_METHOD(void, on_destroy_program, (mge::program * p), ());

    MOCK_METHOD(mge::image_ref, screenshot, (), ());

    MOCK_METHOD(mge::texture_ref, create_texture, (mge::texture_type), ());
};
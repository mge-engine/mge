// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx12_test.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/vertex_buffer.hpp"

class vertex_buffer_test : public mge::dx12::dx12test
{};

TEST_F(vertex_buffer_test, create)
{
    auto& context = m_window->render_context();

    mge::vertex_layout layout;
    layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));
    auto buffer =
        context.create_vertex_buffer(layout, 9 * sizeof(float), nullptr);

    EXPECT_TRUE(buffer);
}

TEST_F(vertex_buffer_test, map_unmap)
{
    auto& context = m_window->render_context();
    float triangle_coords[] = {
        0.0f,
        0.5f,
        0.0f,
        0.45f,
        -0.5,
        0.0f,
        -0.45f,
        -0.5f,
        0.0f,
    };
    mge::vertex_layout layout;
    layout.push_back(mge::vertex_format(mge::data_type::FLOAT, 3));
    auto buffer =
        context.create_vertex_buffer(layout, sizeof(triangle_coords), nullptr);

    int* data = static_cast<int*>(buffer->map());
    memcpy(data, triangle_coords, sizeof(triangle_coords));
    buffer->unmap();
}
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx12_test.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/render_context.hpp"

class index_buffer_test : public mge::dx12::dx12test
{};

TEST_F(index_buffer_test, create)
{
    auto& context = m_window->render_context();

    auto buffer =
        context.create_index_buffer(mge::data_type::INT32, 400, nullptr);

    EXPECT_TRUE(buffer);
}

TEST_F(index_buffer_test, map_unmap)
{
    auto& context = m_window->render_context();

    auto buffer =
        context.create_index_buffer(mge::data_type::INT32, 400, nullptr);

    int* data = static_cast<int*>(buffer->map());
    for (int i = 0; i < 100; ++i) {
        data[i] = i;
    }
    buffer->unmap();
}

TEST_F(index_buffer_test, repeated_create)
{
    auto& context = m_window->render_context();

    int triangle_indices[] = {0, 1, 2};

    for (int i = 0; i < 3; ++i) {
        auto buffer = context.create_index_buffer(mge::data_type::INT32,
                                                  sizeof(triangle_indices),
                                                  triangle_indices);

        EXPECT_EQ(3, buffer->element_count());
    }
}
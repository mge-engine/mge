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

TEST_F(index_buffer_test, construct)
{
    auto& context = m_window->render_context();
    auto  buffer_data = mge::make_buffer(sizeof(int) * 100);
    int*  data_ptr = reinterpret_cast<int*>(buffer_data->data());
    for (int i = 0; i < 100; ++i) {
        data_ptr[i] = i;
    }
    auto buffer =
        context.create_index_buffer(mge::data_type::INT32, 400, buffer_data);
    EXPECT_FALSE(buffer->ready());
    m_window->render_context().frame();
    EXPECT_TRUE(buffer->ready());
    buffer.destroy();
}

TEST_F(index_buffer_test, repeated_create)
{
    auto& context = m_window->render_context();

    int triangle_indices[] = {0, 1, 2};

    mge::buffer_ref indices = mge::make_buffer(triangle_indices);

    for (int i = 0; i < 3; ++i) {
        auto buffer = context.create_index_buffer(mge::data_type::INT32,
                                                  sizeof(triangle_indices),
                                                  indices);

        EXPECT_EQ(3, buffer->element_count());
    }
}
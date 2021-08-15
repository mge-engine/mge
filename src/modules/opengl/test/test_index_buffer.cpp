// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/render_context.hpp"
#include "opengl_test.hpp"

class index_buffer_test : public mge::opengl::opengltest
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
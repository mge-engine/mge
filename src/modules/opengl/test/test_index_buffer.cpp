// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/render_context.hpp"

#include "opengl_test.hpp"

#include "test/benchmark.hpp"

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

TEST_F(index_buffer_test, bench_index_buffer_create_map_unmap)
{
    auto& context = m_window->render_context();
    int*  data = new int[100];
    for (int i = 0; i < 100; ++i) {
        data[i] = i;
    }
    mge::benchmark().run("index_buffer_creation", [&]() {
        auto buffer =
            context.create_index_buffer(mge::data_type::INT32, 400, nullptr);
        int* buffer_data = static_cast<int*>(buffer->map());
        memcpy(buffer_data, data, 100 * sizeof(int));
        buffer->unmap();
    });
}
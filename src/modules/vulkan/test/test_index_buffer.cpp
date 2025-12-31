// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/render_context.hpp"

#include "vulkan_test.hpp"

#include "test/benchmark.hpp"

class index_buffer_test : public mge::vulkan::vulkantest
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
    auto  buffer_data = mge::make_buffer(sizeof(int) * 100);
    int*  data_ptr = reinterpret_cast<int*>(buffer_data->data());
    for (int i = 0; i < 100; ++i) {
        data_ptr[i] = i;
    }
    auto buffer =
        context.create_index_buffer(mge::data_type::INT32, 400, buffer_data);
    EXPECT_TRUE(buffer);
    EXPECT_TRUE(buffer->ready()); // Vulkan index buffer is ready immediately
    m_window->render_context().frame();
    EXPECT_TRUE(buffer->ready());
    buffer.destroy();
}

TEST_F(index_buffer_test, bench_index_buffer_create_map_unmap)
{
    auto& context = m_window->render_context();
    auto  buffer_data = mge::make_buffer(sizeof(int) * 100);
    int*  data = reinterpret_cast<int*>(buffer_data->data());
    for (int i = 0; i < 100; ++i) {
        data[i] = i;
    }
    mge::benchmark().run("index_buffer_creation", [&]() {
        auto buffer = context.create_index_buffer(mge::data_type::INT32,
                                                  400,
                                                  buffer_data);
        m_window->render_context().frame();
        buffer.destroy();
    });
}

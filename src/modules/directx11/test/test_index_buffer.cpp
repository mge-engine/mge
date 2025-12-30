// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/index_buffer.hpp"
#include "mge/graphics/render_context.hpp"

class index_buffer_test : public mge::dx11::dx11test
{};

TEST_F(index_buffer_test, create)
{
    auto& context = m_window->render_context();

    auto buffer =
        context.create_index_buffer(mge::data_type::INT32, 400, nullptr);

    EXPECT_TRUE(buffer);
}

TEST_F(index_buffer_test, set_data)
{
    auto& context = m_window->render_context();

    auto buffer = context.create_index_buffer(mge::data_type::INT32, 400);
    FAIL() << "Setting data on DirectX11 index buffer not implemented yet";
}
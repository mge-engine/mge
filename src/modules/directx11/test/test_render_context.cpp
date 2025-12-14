// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/render_context.hpp"

class render_context_test : public mge::dx11::dx11test
{};

TEST_F(render_context_test, extent)
{
    auto& context = m_window->render_context();
    EXPECT_EQ(context.extent().width, 800u);
    EXPECT_EQ(context.extent().height, 600u);
}

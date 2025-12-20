// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "vulkan_test.hpp"

class render_context_test : public mge::vulkan::vulkantest
{};

TEST_F(render_context_test, extent)
{
    auto& context = m_window->render_context();
    EXPECT_EQ(context.extent().width, 800u);
    EXPECT_EQ(context.extent().height, 600u);
}

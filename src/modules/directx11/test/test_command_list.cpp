// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/render_context.hpp"

class command_list_test : public mge::dx11::dx11test
{};

TEST_F(command_list_test, create)
{
    auto& context = m_window->render_context();

    auto command_list = context.create_command_list();

    EXPECT_TRUE(command_list);
}

// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx12_test.hpp"

class render_system_test : public mge::dx12::dx12test
{};

TEST_F(render_system_test, monitors)
{
    auto monitors = m_render_system->monitors();
    EXPECT_GE(monitors.size(), 1u);
}

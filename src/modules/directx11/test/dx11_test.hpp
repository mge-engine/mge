/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#pragma once

#include "test/googlemock.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"

namespace dx11
{
class dx11test : public ::testing::Test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    static mge::render_system_ref render_system;
};

class dx11test_with_window : public dx11test
{
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    static mge::window_ref window;
};
}

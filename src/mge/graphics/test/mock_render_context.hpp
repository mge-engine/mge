// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "test/googlemock.hpp"

class MOCK_render_context : public mge::render_context
{
public:
    MOCK_render_context() {}
    ~MOCK_render_context() {}
};
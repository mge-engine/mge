// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/program.hpp"
#include "test/googlemock.hpp"

class MOCK_program : public mge::program
{
public:
    MOCK_program(mge::render_context& context)
        : mge::program(context)
    {}

    ~MOCK_program() {}

    MOCK_METHOD(void, on_link, (), ());
    MOCK_METHOD(void, on_set_shader, (const mge::shader_ref&), ());
};
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/shader.hpp"
#include "test/googlemock.hpp"

class MOCK_shader : public mge::shader
{
public:
    MOCK_shader(mge::render_context& context, mge::shader_type type)
        : mge::shader(context, type)
    {}

    ~MOCK_shader() {}

    MOCK_METHOD(void, on_compile, (const std::string_view source), (override));
    MOCK_METHOD(void, on_set_code, (const mge::buffer& code), (override));
};

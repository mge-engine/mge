// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader_program.hpp"
#include "test/googlemock.hpp"

namespace mge {
    class mock_shader_program : public shader_program
    {
    public:
        mock_shader_program(render_context &context, shader_type type)
            : shader_program(context, type)
        {}

        ~mock_shader_program() = default;

        MOCK_METHOD1(on_compile, void(const std::string &));
        MOCK_METHOD1(on_load, void(const mge::buffer &));
    };
} // namespace mge

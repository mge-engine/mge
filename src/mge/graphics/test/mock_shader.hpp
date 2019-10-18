// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/shader.hpp"

namespace mge {
    class mock_shader : public shader
    {
    public:
        mock_shader(render_context& context,
                    shader_type type)
            :shader(context, type)
        {}

        ~mock_shader() = default;

        MOCK_METHOD1(on_compile, void(const std::string&));
        MOCK_METHOD1(on_load, void(const mge::buffer&));
    };
}
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/pipeline.hpp"

namespace mge {
    class mock_pipeline : public pipeline
    {
    public:
        mock_pipeline(render_context& context)
            :pipeline(context)
        {}

        ~mock_pipeline() = default;

        MOCK_METHOD1(on_set_shader_program, void(const shader_program_ref&));
        MOCK_METHOD0(on_link, void());
    };
}

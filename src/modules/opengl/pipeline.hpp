// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/pipeline.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context;

    class pipeline : public mge::pipeline
    {
    public:
        pipeline(render_context& context);
        virtual ~pipeline();

        void on_link() override;
        void on_set_shader(const shader_ref& shader) override;

    private:
        void dump_info_log();

        GLuint m_program;
    };
} // namespace mge::opengl
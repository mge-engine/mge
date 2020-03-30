// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/pipeline.hpp"
#include <array>
namespace dx11 {
    class shader_program;

    class pipeline : public mge::pipeline
    {
    public:
        pipeline(mge::render_context &context);
        virtual ~pipeline();

    protected:
        void on_link() override;
        void
        on_set_shader_program(const mge::shader_program_ref &shader) override;

    private:
        void reflect_vertex_shader(dx11::shader_program *s);

        std::array<mge::shader_program_ref,
                   (size_t)mge::shader_type::MAX_SHADER_TYPE + 1>
               m_shaders;
        size_t m_vertex_shader_uniform_count;
    };

} // namespace dx11

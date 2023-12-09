// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "glslang.hpp"
#include "mge/graphics/program.hpp"

namespace mge::vulkan {
    class render_context;
    class program : public mge::program
    {
    public:
        program(render_context& context);
        ~program();

    protected:
        virtual void on_link() override;
        virtual void on_set_shader(const shader_ref& shader) override;

    private:
        glslang_program_t* m_program;
    };
} // namespace mge::vulkan
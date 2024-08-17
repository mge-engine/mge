// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/program.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;
    class shader;
    class program : public mge::program
    {
    public:
        program(render_context& context);
        ~program();
        const auto& shader_stage_create_infos() const
        {
            return m_shader_stage_create_infos;
        }

    protected:
        virtual void on_link() override;
        virtual void on_set_shader(const shader_ref& shader) override;

        std::vector<VkPipelineShaderStageCreateInfo>
            m_shader_stage_create_infos;
        std::vector<std::shared_ptr<mge::vulkan::shader>> m_shaders;
    };
} // namespace mge::vulkan
// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "glslang.hpp"
#include "mge/graphics/shader.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;

    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();
        const VkPipelineShaderStageCreateInfo& pipeline_stage_info() const;

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

    private:
        glslang_stage_t       stage() const;
        void                  create_shader_module();
        void                  destroy_shader_module();
        VkShaderStageFlagBits vk_stage_flags() const;

        render_context&                 m_vulkan_context;
        VkShaderModule                  m_shader_module;
        mge::buffer                     m_code;
        VkPipelineShaderStageCreateInfo m_pipeline_stage_info;
    };
} // namespace mge::vulkan
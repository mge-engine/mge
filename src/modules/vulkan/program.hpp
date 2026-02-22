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

        VkPipelineLayout pipeline_layout() const noexcept
        {
            return m_pipeline_layout;
        }

        VkDescriptorSetLayout descriptor_set_layout() const noexcept
        {
            return m_descriptor_set_layout;
        }

        struct sampler_binding
        {
            std::string name;
            uint32_t    binding;
        };

        using sampler_binding_list = std::vector<sampler_binding>;

        const sampler_binding_list& sampler_bindings() const noexcept
        {
            return m_sampler_bindings;
        }

    protected:
        virtual void on_link() override;
        virtual void on_set_shader(mge::shader* shader) override;

    private:
        void create_pipeline_layout();

        std::vector<VkPipelineShaderStageCreateInfo>
                                          m_shader_stage_create_infos;
        std::vector<mge::vulkan::shader*> m_shaders;
        VkPipelineLayout                  m_pipeline_layout{VK_NULL_HANDLE};
        VkDescriptorSetLayout m_descriptor_set_layout{VK_NULL_HANDLE};
        sampler_binding_list  m_sampler_bindings;
    };
} // namespace mge::vulkan
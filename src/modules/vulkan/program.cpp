// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "error.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge::vulkan {

    program::program(render_context& context)
        : mge::program(context)
    {}

    program::~program()
    {
        if (m_descriptor_set_layout != VK_NULL_HANDLE) {
            static_cast<render_context&>(context())
                .vkDestroyDescriptorSetLayout(
                    static_cast<render_context&>(context()).device(),
                    m_descriptor_set_layout,
                    nullptr);
            m_descriptor_set_layout = VK_NULL_HANDLE;
        }
        if (m_pipeline_layout != VK_NULL_HANDLE) {
            static_cast<render_context&>(context()).vkDestroyPipelineLayout(
                static_cast<render_context&>(context()).device(),
                m_pipeline_layout,
                nullptr);
            m_pipeline_layout = VK_NULL_HANDLE;
        }
    }

    void program::on_link()
    {
        m_shader_stage_create_infos.clear();
        m_uniform_block_metadata.clear();
        m_attributes.clear();
        m_uniforms.clear();
        m_sampler_bindings.clear();

        std::vector<std::pair<std::string, uint32_t>> sampler_bindings_raw;
        for (const auto& shader : m_shaders) {
            m_shader_stage_create_infos.push_back(
                shader->pipeline_stage_info());
            shader->reflect(m_attributes,
                            m_uniforms,
                            m_uniform_block_metadata,
                            sampler_bindings_raw);
        }
        for (const auto& [name, binding] : sampler_bindings_raw) {
            m_sampler_bindings.push_back({name, binding});
        }
        create_pipeline_layout();
    }

    void program::on_set_shader(mge::shader* s)
    {
        auto vulkan_shader = static_cast<mge::vulkan::shader*>(s);
        m_shaders.push_back(vulkan_shader);
    }

    void program::create_pipeline_layout()
    {
        std::vector<VkDescriptorSetLayoutBinding> layout_bindings;

        // Create descriptor set layout bindings for uniform buffers
        for (const auto& ub : m_uniform_block_metadata) {
            VkDescriptorSetLayoutBinding binding = {};
            binding.binding = ub.location;
            binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            binding.descriptorCount = 1;
            binding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
            binding.pImmutableSamplers = nullptr;
            layout_bindings.push_back(binding);
        }

        // Create descriptor set layout bindings for combined image samplers
        for (const auto& sb : m_sampler_bindings) {
            VkDescriptorSetLayoutBinding binding = {};
            binding.binding = sb.binding;
            binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            binding.descriptorCount = 1;
            binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            binding.pImmutableSamplers = nullptr;
            layout_bindings.push_back(binding);
        }

        VkDescriptorSetLayout descriptor_set_layout = VK_NULL_HANDLE;
        if (!layout_bindings.empty()) {
            VkDescriptorSetLayoutCreateInfo layout_info = {};
            layout_info.sType =
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layout_info.bindingCount =
                static_cast<uint32_t>(layout_bindings.size());
            layout_info.pBindings = layout_bindings.data();

            CHECK_VK_CALL(
                static_cast<render_context&>(context())
                    .vkCreateDescriptorSetLayout(
                        static_cast<render_context&>(context()).device(),
                        &layout_info,
                        nullptr,
                        &descriptor_set_layout));
            m_descriptor_set_layout = descriptor_set_layout;
        }

        VkPipelineLayoutCreateInfo pipeline_layout_info = {};
        pipeline_layout_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount =
            descriptor_set_layout != VK_NULL_HANDLE ? 1 : 0;
        pipeline_layout_info.pSetLayouts =
            descriptor_set_layout != VK_NULL_HANDLE ? &descriptor_set_layout
                                                    : nullptr;
        pipeline_layout_info.pushConstantRangeCount = 0;    // Optional
        pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

        CHECK_VK_CALL(
            static_cast<render_context&>(context()).vkCreatePipelineLayout(
                static_cast<render_context&>(context()).device(),
                &pipeline_layout_info,
                nullptr,
                &m_pipeline_layout));
    }
} // namespace mge::vulkan
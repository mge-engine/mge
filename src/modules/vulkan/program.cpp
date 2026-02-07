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
        for (const auto& shader : m_shaders) {
            m_shader_stage_create_infos.push_back(
                shader->pipeline_stage_info());
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
        VkPipelineLayoutCreateInfo pipeline_layout_info = {};
        pipeline_layout_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_info.setLayoutCount = 0;            // Optional
        pipeline_layout_info.pSetLayouts = nullptr;         // Optional
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
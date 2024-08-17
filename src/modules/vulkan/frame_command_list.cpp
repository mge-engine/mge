#include "frame_command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "vertex_buffer.hpp"

namespace mge::vulkan {
    frame_command_list::frame_command_list(render_context& context,
                                           uint64_t        frame,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_vulkan_context(context)
        , m_frame(frame)
    {
        VkCommandBufferAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        alloc_info.commandPool = m_vulkan_context.graphics_command_pool();
        alloc_info.commandBufferCount = 1;

        CHECK_VK_CALL(
            m_vulkan_context.vkAllocateCommandBuffers(m_vulkan_context.device(),
                                                      &alloc_info,
                                                      &m_command_buffer));
        begin();
    }

    frame_command_list::~frame_command_list()
    {
        if (m_command_buffer) {
            m_vulkan_context.discard_command_buffer(m_frame, m_command_buffer);
            m_command_buffer = VK_NULL_HANDLE;
        }
    }

    void frame_command_list::begin()
    {
        // initially reset command buffer
        CHECK_VK_CALL(
            m_vulkan_context.vkResetCommandBuffer(m_command_buffer, 0));
        // begin, inheriting the render pass
        VkCommandBufferInheritanceInfo inheritance_info = {};
        inheritance_info.sType =
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritance_info.renderPass = m_vulkan_context.render_pass();
        inheritance_info.subpass = 0;
        inheritance_info.framebuffer =
            m_vulkan_context.framebuffer(backbuffer_index());
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        begin_info.pInheritanceInfo = &inheritance_info;
        CHECK_VK_CALL(m_vulkan_context.vkBeginCommandBuffer(m_command_buffer,
                                                            &begin_info));
    }

    void frame_command_list::viewport(const mge::viewport& vp)
    {
        const VkViewport* p_vk_cp =
            reinterpret_cast<const VkViewport*>(vp.data());
        m_vulkan_context.vkCmdSetViewport(m_command_buffer, 0, 1, p_vk_cp);
    }

    void frame_command_list::scissor(const mge::rectangle& r)
    {
        VkRect2D vk_cp = {};
        vk_cp.offset.x = static_cast<int32_t>(r.left);
        vk_cp.offset.y = static_cast<int32_t>(r.top);
        vk_cp.extent.width = static_cast<int32_t>(r.width());
        vk_cp.extent.height = static_cast<int32_t>(r.height());
        m_vulkan_context.vkCmdSetScissor(m_command_buffer, 0, 1, &vk_cp);
    }

    void frame_command_list::default_scissor()
    {
        VkRect2D vk_cp = {};
        vk_cp.offset = {0, 0};
        vk_cp.extent = m_vulkan_context.extent();
        m_vulkan_context.vkCmdSetScissor(m_command_buffer, 0, 1, &vk_cp);
    }

    void frame_command_list::clear(const mge::rgba_color& c)
    {
        /*
        VkClearValue clear_color = {};
        clear_color.color = {{c.r, c.g, c.b, c.a}};
        VkClearAttachment clear_attachment = {};
        clear_attachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        clear_attachment.colorAttachment = 0;
        clear_attachment.clearValue = clear_color;
        VkClearRect clear_rect = {};
        clear_rect.rect.offset = {0, 0};
        clear_rect.rect.extent = m_vulkan_context.extent();
        clear_rect.baseArrayLayer = 0;
        clear_rect.layerCount = 1;
        m_vulkan_context.vkCmdClearAttachments(m_command_buffer,
                                               1,
                                               &clear_attachment,
                                               1,
                                               &clear_rect);
        */
    }

    void frame_command_list::draw(const mge::draw_command& command)
    {
        mge::vulkan::program* draw_program =
            static_cast<mge::vulkan::program*>(command.program().get());
        mge::vulkan::vertex_buffer* vertex_buffer =
            static_cast<mge::vulkan::vertex_buffer*>(command.vertices().get());
        mge::vulkan::index_buffer* index_buffer =
            static_cast<mge::vulkan::index_buffer*>(command.indices().get());

        VkDynamicState dynamic_states[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                           VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
        dynamic_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = 2;
        dynamic_state_create_info.pDynamicStates = dynamic_states;

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info =
            {};
        vertex_input_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
        vertex_input_state_create_info.pVertexBindingDescriptions =
            &vertex_buffer->binding_description();
        vertex_input_state_create_info.vertexAttributeDescriptionCount =
            static_cast<uint32_t>(
                vertex_buffer->attribute_descriptions().size());
        vertex_input_state_create_info.pVertexAttributeDescriptions =
            vertex_buffer->attribute_descriptions().data();

        VkPipelineInputAssemblyStateCreateInfo
            input_assembly_state_create_info = {};
        input_assembly_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        // TODO: Vulkan: use topology from draw_command
        input_assembly_state_create_info.topology =
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info =
            {};
        rasterization_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable =
            VK_FALSE; // discard elements out of depth range
        rasterization_state_create_info.rasterizerDiscardEnable =
            VK_FALSE; // allow elements to pass rasterizer stage
        rasterization_state_create_info.polygonMode =
            VK_POLYGON_MODE_FILL; // fill the polygon drawn
        rasterization_state_create_info.lineWidth = 1.0f;

        rasterization_state_create_info.cullMode =
            VK_CULL_MODE_BACK_BIT; // cull back faces
        rasterization_state_create_info.frontFace =
            VK_FRONT_FACE_CLOCKWISE; // clockwise front face

        // no depth bias, that's only used in shadow mapping
        rasterization_state_create_info.depthBiasEnable = VK_FALSE;
        rasterization_state_create_info.depthBiasConstantFactor = 0.0f;
        rasterization_state_create_info.depthBiasClamp = 0.0f;
        rasterization_state_create_info.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling_create_info{};
        multisampling_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling_create_info.sampleShadingEnable = VK_FALSE;
        multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling_create_info.minSampleShading = 1.0f;
        multisampling_create_info.pSampleMask = nullptr;
        multisampling_create_info.alphaToCoverageEnable = VK_FALSE;
        multisampling_create_info.alphaToOneEnable = VK_FALSE;

        // no depth stencil tests
        // VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info
        // =
        //    {};
        // depth_stencil_state_create_info.sType =
        //    VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

        // color blending
        VkPipelineColorBlendAttachmentState color_blend_attachment_state = {};
        color_blend_attachment_state.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment_state.blendEnable = VK_FALSE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {};
        color_blend_state_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable = VK_FALSE;
        color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments =
            &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        // pipeline layout
        VkPipelineLayout           pipeline_layout{VK_NULL_HANDLE};
        VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
        pipeline_layout_create_info.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = 0;
        pipeline_layout_create_info.pSetLayouts = nullptr;
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges = nullptr;

        CHECK_VK_CALL(m_vulkan_context.vkCreatePipelineLayout(
            m_vulkan_context.device(),
            &pipeline_layout_create_info,
            nullptr,
            &pipeline_layout));

        VkGraphicsPipelineCreateInfo pipeline_create_info = {};
        pipeline_create_info.sType =
            VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_create_info.stageCount = static_cast<uint32_t>(
            draw_program->shader_stage_create_infos().size());
        pipeline_create_info.pStages =
            draw_program->shader_stage_create_infos().data();
        pipeline_create_info.pVertexInputState =
            &vertex_input_state_create_info;
        pipeline_create_info.pInputAssemblyState =
            &input_assembly_state_create_info;
        pipeline_create_info.pViewportState = &viewport_state_create_info;
        pipeline_create_info.pRasterizationState =
            &rasterization_state_create_info;
        pipeline_create_info.pMultisampleState = &multisampling_create_info;
        pipeline_create_info.pDepthStencilState = nullptr;
        pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
        pipeline_create_info.pDynamicState = &dynamic_state_create_info;
        pipeline_create_info.layout = pipeline_layout;
        pipeline_create_info.renderPass = m_vulkan_context.render_pass();
        pipeline_create_info.subpass = 0;
        pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_create_info.basePipelineIndex = -1;

        VkPipeline pipeline{VK_NULL_HANDLE};
        CHECK_VK_CALL(m_vulkan_context.vkCreateGraphicsPipelines(
            m_vulkan_context.device(),
            VK_NULL_HANDLE,
            1,
            &pipeline_create_info,
            nullptr,
            &pipeline));

        VkViewport vp = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0};
        vp.width = static_cast<float>(m_vulkan_context.extent().width);
        vp.height = static_cast<float>(m_vulkan_context.extent().height);

        m_vulkan_context.vkCmdSetViewport(m_command_buffer, 0, 1, &vp);

        m_vulkan_context.vkCmdBindPipeline(m_command_buffer,
                                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                                           pipeline);
        VkDeviceSize offsets[1]{0};
        VkBuffer     buffers[1]{vertex_buffer->vk_buffer()};
        m_vulkan_context.vkCmdBindVertexBuffers(m_command_buffer,
                                                0,
                                                1,
                                                buffers,
                                                offsets);
        m_vulkan_context.vkCmdBindIndexBuffer(m_command_buffer,
                                              index_buffer->vk_buffer(),
                                              0,
                                              index_buffer->vk_index_type());
        m_vulkan_context.vkCmdDrawIndexed(
            m_command_buffer,
            static_cast<uint32_t>(index_buffer->element_count()),
            1,
            0,
            0,
            0);
#if 0
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &uniformBuffers[currentFrame].descriptorSet, 0, nullptr);
		// Bind the rendering pipeline
		// The pipeline (state object) contains all states of the rendering pipeline, binding it will set all the states specified at pipeline creation time
		
		// Bind triangle vertex buffer (contains position and colors)
		
		// Bind triangle index buffer
		vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
		// Draw indexed triangle
		vkCmdDrawIndexed(commandBuffer, indices.count, 1, 0, 0, 1);
#endif
        m_vulkan_context.discard_pipeline(m_frame, pipeline);
        m_vulkan_context.vkDestroyPipelineLayout(m_vulkan_context.device(),
                                                 pipeline_layout,
                                                 nullptr);
    }

    void frame_command_list::execute()
    {
        m_vulkan_context.execute_frame_command_buffer(m_command_buffer);
    }

    void frame_command_list::finish()
    {
        CHECK_VK_CALL(m_vulkan_context.vkEndCommandBuffer(m_command_buffer));
    }
} // namespace mge::vulkan
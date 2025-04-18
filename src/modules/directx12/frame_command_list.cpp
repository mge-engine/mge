// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "frame_command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"

#include <sstream>

namespace mge::dx12 {
    frame_command_list::frame_command_list(render_context& context,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_dx12_context(context)
    {
        m_command_allocator = m_dx12_context.create_command_allocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT);

        m_command_list = m_dx12_context.create_dx12_command_list(
            m_command_allocator.Get(),
            D3D12_COMMAND_LIST_TYPE_DIRECT);

        auto rtv_handle = m_dx12_context.rtv_handle(backbuffer_index);
        m_command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, nullptr);

        m_command_list->RSSetScissorRects(1, &m_dx12_context.scissor_rect());
        m_command_list->RSSetViewports(1, &m_dx12_context.viewport());

        m_rasterizer_desc = {
            .FillMode = D3D12_FILL_MODE_SOLID,
            .CullMode = D3D12_CULL_MODE_BACK,
            .FrontCounterClockwise = FALSE,
            .DepthBias = D3D12_DEFAULT_DEPTH_BIAS,
            .DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
            .SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
            .DepthClipEnable = TRUE,
            .MultisampleEnable = FALSE,
            .AntialiasedLineEnable = FALSE,
            .ForcedSampleCount = 0,
            .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF};

        m_blend_desc = {.AlphaToCoverageEnable = FALSE,
                        .IndependentBlendEnable = FALSE};

        m_blend_desc.RenderTarget[0] = {.BlendEnable = FALSE,
                                        .LogicOpEnable = FALSE,
                                        .SrcBlend = D3D12_BLEND_ONE,
                                        .DestBlend = D3D12_BLEND_ZERO,
                                        .BlendOp = D3D12_BLEND_OP_ADD,
                                        .SrcBlendAlpha = D3D12_BLEND_ONE,
                                        .DestBlendAlpha = D3D12_BLEND_ZERO,
                                        .BlendOpAlpha = D3D12_BLEND_OP_ADD,
                                        .LogicOp = D3D12_LOGIC_OP_NOOP,
                                        .RenderTargetWriteMask =
                                            D3D12_COLOR_WRITE_ENABLE_ALL};
    }

    frame_command_list::~frame_command_list() {}

    void frame_command_list::scissor(const mge::rectangle& r)
    {
        D3D12_RECT rect = {.left = static_cast<LONG>(r.left),
                           .top = static_cast<LONG>(r.top),
                           .right = static_cast<LONG>(r.right),
                           .bottom = static_cast<LONG>(r.bottom)};
        m_command_list->RSSetScissorRects(1, &rect);
    }

    void frame_command_list::viewport(const mge::viewport& vp)
    {
        D3D12_VIEWPORT viewport = {.TopLeftX = vp.x,
                                   .TopLeftY = vp.y,
                                   .Width = vp.width,
                                   .Height = vp.height,
                                   .MinDepth = vp.min_depth,
                                   .MaxDepth = vp.max_depth};
        m_command_list->RSSetViewports(1, &viewport);
    }

    void frame_command_list::default_scissor()
    {
        m_command_list->RSSetScissorRects(1, &m_dx12_context.scissor_rect());
    }

    void frame_command_list::clear(const mge::rgba_color& c)
    {
        auto rtv_handle = m_dx12_context.rtv_handle(backbuffer_index());
        m_command_list->ClearRenderTargetView(rtv_handle, c.data(), 0, nullptr);
    }

    void frame_command_list::clear_depth(float depth)
    {
        auto dsv_handle = m_dx12_context.dsv_handle(backbuffer_index());
        m_command_list->ClearDepthStencilView(dsv_handle,
                                              D3D12_CLEAR_FLAG_DEPTH,
                                              depth,
                                              0,
                                              0,
                                              nullptr);
    }
    void frame_command_list::clear_stencil(int32_t stencil)
    {
        auto dsv_handle = m_dx12_context.dsv_handle(backbuffer_index());
        m_command_list->ClearDepthStencilView(dsv_handle,
                                              D3D12_CLEAR_FLAG_STENCIL,
                                              0.0f,
                                              static_cast<UINT8>(stencil),
                                              0,
                                              nullptr);
    }

    void frame_command_list::draw(const mge::draw_command& command)
    {
        auto vs_ref = dx12_program(*command.program())
                          .program_shader(shader_type::VERTEX);
        auto ps_ref = dx12_program(*command.program())
                          .program_shader(shader_type::FRAGMENT);
        auto& vs = dx12_shader(*vs_ref);
        auto& ps = dx12_shader(*ps_ref);

        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.InputLayout = {vs.input_layout(), vs.input_layout_count()};
        pso_desc.pRootSignature =
            dx12_program(*command.program()).root_signature();
        pso_desc.VS = {vs.code()->GetBufferPointer(),
                       vs.code()->GetBufferSize()};
        pso_desc.PS = {ps.code()->GetBufferPointer(),
                       ps.code()->GetBufferSize()};

        pso_desc.RasterizerState = m_rasterizer_desc;
        pso_desc.BlendState = m_blend_desc;
        pso_desc.DepthStencilState = {.DepthEnable = false,
                                      .StencilEnable = false};
        pso_desc.SampleMask = UINT_MAX;
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        pso_desc.SampleDesc = {.Count = 1, .Quality = 0};

        auto root_signature = dx12_program(*command.program()).root_signature();
        mge::com_ptr<ID3D12PipelineState> pipeline_state;
        auto rc = m_dx12_context.device()->CreateGraphicsPipelineState(
            &pso_desc,
            IID_PPV_ARGS(&pipeline_state));

        CHECK_HRESULT(rc, ID3D12Device, CreateGraphicsPipelineState);

        m_command_list->SetGraphicsRootSignature(root_signature);
        m_command_list->SetPipelineState(pipeline_state.Get());
        m_command_list->IASetPrimitiveTopology(
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_command_list->IASetVertexBuffers(
            0,
            1,
            &dx12_vertex_buffer(*command.vertices()).view());
        m_command_list->IASetIndexBuffer(
            &dx12_index_buffer(*command.indices()).view());
        m_command_list->DrawIndexedInstanced(
            static_cast<UINT>(command.indices()->element_count()),
            1,
            0,
            0,
            0);
        m_dx12_context.add_frame_resource(pipeline_state);
    }

    void frame_command_list::execute()
    {
        m_dx12_context.execute_dx12_command_list(m_command_list.Get());
        m_dx12_context.add_frame_resource(m_command_allocator);
        m_dx12_context.add_frame_resource(m_command_list);
    }

    void frame_command_list::finish()
    {
        m_command_list->Close();
    }

} // namespace mge::dx12

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "program.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge::dx12 {
#include <d3d12.h> // Include the necessary header file

    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
        , m_dx12_context(context)
        , m_color_set(false)
    {
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

        HRESULT rc = m_dx12_context.device()->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_BUNDLE,
            IID_PPV_ARGS(&m_bundle_allocator));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);
    }

    command_list::~command_list() { release_draw_list(); }

    void command_list::release_draw_list()
    {
        for (auto& d : m_draw_list) {
            if (std::get<1>(d)) {
                std::get<1>(d)->Release();
            }
            if (std::get<2>(d)) {
                std::get<2>(d)->Release();
            }
        }
        m_draw_list.clear();
    }

    void command_list::clear(const rgba_color& c)
    {
        m_color_set = true;
        m_clear_color = c;
    }

    void command_list::draw(const mge::draw_command& command)
    {
        ID3D12CommandList* command_list = nullptr;

        auto rc = m_dx12_context.device()->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_BUNDLE,
            m_bundle_allocator.Get(),
            nullptr,
            IID_PPV_ARGS(&command_list));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandList);

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
        ID3D12PipelineState* pipeline_state = nullptr;
        try {
            rc = m_dx12_context.device()->CreateGraphicsPipelineState(
                &pso_desc,
                IID_PPV_ARGS(&pipeline_state));

            CHECK_HRESULT(rc, ID3D12Device, CreateGraphicsPipelineState);

            m_draw_list.push_back(std::make_tuple(
                dx12_program(*command.program()).root_signature(),
                pipeline_state,
                command_list));
        } catch (...) {
            if (pipeline_state) {
                pipeline_state->Release();
            }
            command_list->Release();
            throw;
        }
    }

    void command_list::execute() { m_dx12_context.execute(*this); }

    bool command_list::empty() const { return true; }

} // namespace mge::dx12
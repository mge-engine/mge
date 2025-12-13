// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "program.hpp"
#include "vertex_buffer.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
        , m_dx11_context(context)
    {}

    void command_list::start_recording()
    {
        if (is_recording()) {
            return;
        }

        ID3D11DeviceContext* deferred_context;

        auto hr =
            m_dx11_context.device()->CreateDeferredContext(0,
                                                           &deferred_context);
        CHECK_HRESULT(hr, ID3D11Device, CreateDeferredContext);
        m_deferred_context.reset(deferred_context);
        m_dx11_context.setup_context(*m_deferred_context);
    }

    command_list::~command_list()
    {
        if (is_recording()) {
            MGE_WARNING_TRACE(DX11, "Command list destroyed while recording");
        }
    }

    void command_list::viewport(const mge::viewport& vp)
    {
        start_recording();
        D3D11_VIEWPORT dx11_vp = {
            .TopLeftX = vp.x,
            .TopLeftY = vp.y - vp.height,
            .Width = vp.width,
            .Height = vp.height,
            .MinDepth = vp.min_depth,
            .MaxDepth = vp.max_depth,
        };
        m_deferred_context->RSSetViewports(1, &dx11_vp);
    }

    void command_list::scissor(const rectangle& rect)
    {
        start_recording();
        D3D11_RECT dx11_rect = {
            .left = static_cast<LONG>(rect.left),
            .top = static_cast<LONG>(rect.top),
            .right = static_cast<LONG>(rect.right),
            .bottom = static_cast<LONG>(rect.bottom),
        };
        m_deferred_context->RSSetScissorRects(1, &dx11_rect);
    }

    void command_list::default_scissor()
    {
        scissor(m_dx11_context.default_scissor());
    }

    void command_list::clear(const rgba_color& c)
    {
        start_recording();
        float clearcolor[4] = {c.r, c.g, c.b, c.a};
        m_deferred_context->ClearRenderTargetView(
            m_dx11_context.render_target_view(),
            clearcolor);
    }

    void command_list::clear_depth(float depth)
    {
        start_recording();
        m_deferred_context->ClearDepthStencilView(
            m_dx11_context.depth_stencil_view(),
            D3D11_CLEAR_DEPTH,
            depth,
            0);
    }

    void command_list::clear_stencil(int32_t stencil)
    {
        start_recording();
        m_deferred_context->ClearDepthStencilView(
            m_dx11_context.depth_stencil_view(),
            D3D11_CLEAR_STENCIL,
            0.0f,
            static_cast<UINT8>(stencil));
    }

    void command_list::draw(const mge::draw_command& command)
    {
        const dx11::program* dx11_program =
            static_cast<const dx11 ::program*>(command.program().get());

        const dx11::shader* dx11_vertex_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::VERTEX).get());

        ID3D11InputLayout* input_layout = dx11_vertex_shader->input_layout();
        if (input_layout == nullptr) {
            MGE_THROW(mge::illegal_state)
                << "No input layout for vertex shader";
        }
        m_deferred_context->IASetInputLayout(input_layout);

        const dx11::vertex_buffer* dx11_vertex_buffer =
            static_cast<const dx11::vertex_buffer*>(command.vertices());
        UINT element_size =
            static_cast<UINT>(dx11_vertex_buffer->layout().binary_size());
        UINT          stride = 0;
        ID3D11Buffer* vertex_buffer = dx11_vertex_buffer->buffer();
        m_deferred_context->IASetVertexBuffers(0,
                                               1,
                                               &vertex_buffer,
                                               &element_size,
                                               &stride);

        const dx11::index_buffer* dx11_index_buffer =
            static_cast<const dx11::index_buffer*>(command.indices());
        ID3D11Buffer* index_buffer = dx11_index_buffer->buffer();
        m_deferred_context->IASetIndexBuffer(index_buffer,
                                             DXGI_FORMAT_R32_UINT,
                                             0);

        m_deferred_context->VSSetShader(
            dx11_vertex_shader->directx_vertex_shader(),
            nullptr,
            0);

        const dx11::shader* dx11_pixel_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::FRAGMENT).get());
        m_deferred_context->PSSetShader(
            dx11_pixel_shader->directx_pixel_shader(),
            nullptr,
            0);
        UINT element_count =
            static_cast<UINT>(dx11_index_buffer->element_count());
        m_deferred_context->DrawIndexed(element_count, 0, 0);
    }

    void command_list::finish()
    {
        if (!is_recording()) {
            MGE_THROW(mge::illegal_state)
                << "Command list not in recording state";
        }
        ID3D11CommandList* command_list = nullptr;
        auto hr = m_deferred_context->FinishCommandList(FALSE, &command_list);
        CHECK_HRESULT(hr, ID3D11DeviceContext, FinishCommandList);
        m_command_list.reset(command_list);
        m_deferred_context.reset();
    }

    void command_list::execute()
    {
        if (is_recording()) {
            finish();
        }
        m_dx11_context.device_context()->ExecuteCommandList(
            m_command_list.get(),
            TRUE);
    }

} // namespace mge::dx11

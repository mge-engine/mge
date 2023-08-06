// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "program.hpp"
#include "vertex_buffer.hpp"

namespace mge::dx11 {

    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
        , m_dx11_context(context)
    {
        ID3D11DeviceContext* deferred_context;
        auto hr = context.device()->CreateDeferredContext(0, &deferred_context);
        CHECK_HRESULT(hr, ID3D11Device, CreateDeferredContext);
        m_deferred_context.reset(deferred_context);
    }

    command_list::~command_list() {}

    void command_list::clear(const rgba_color& c)
    {
        float clearcolor[4] = {c.r, c.g, c.b, c.a};
        m_deferred_context->ClearRenderTargetView(
            m_dx11_context.render_target_view(),
            clearcolor);
    }
#if 0
    static void fill_input_layout(const mge::vertex_layout& layout,
                                  D3D11_INPUT_ELEMENT_DESC* input_layout)
    {
        size_t offset = 0;
        for (const auto& element : layout) {
            D3D11_INPUT_ELEMENT_DESC& desc = *input_layout++;
            /*
            desc.SemanticName =
            desc.SemanticIndex = element.semantic_index();
            desc.Format = static_cast<DXGI_FORMAT>(element.format());
            desc.InputSlot = 0;
            desc.AlignedByteOffset = static_cast<UINT>(offset);
            desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            desc.InstanceDataStepRate = 0;
       vert     offset += element.binary_size();
            */
        }
    }
#endif

    void command_list::draw(const mge::draw_command& command)
    {
        const dx11::program* dx11_program =
            static_cast<const dx11 ::program*>(command.program().get());

        const dx11::shader* dx11_vertex_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::VERTEX).get());

        const dx11::shader* dx11_pixel_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::FRAGMENT).get());

        const dx11::vertex_buffer* dx11_vertex_buffer =
            static_cast<const dx11::vertex_buffer*>(command.vertices().get());

        const auto&               layout = command.vertices()->layout();
        const size_t              layout_size = layout.size();
        D3D11_INPUT_ELEMENT_DESC* input_desc =
            m_dx11_context.layouts().get(layout);
        ID3D11InputLayout* input_layout = nullptr;

        auto rc = m_dx11_context.device()->CreateInputLayout(
            input_desc,
            static_cast<UINT>(layout_size),
            dx11_vertex_shader->code()->GetBufferPointer(),
            dx11_vertex_shader->code()->GetBufferSize(),
            &input_layout);
        CHECK_HRESULT(rc, ID3D11Device, CreateInputLayout);

        UINT          stride = static_cast<UINT>(layout.binary_size());
        UINT          offset = 0;
        ID3D11Buffer* vertex_buffer = dx11_vertex_buffer->buffer();
        m_deferred_context->IASetVertexBuffers(0,
                                               1,
                                               &vertex_buffer,
                                               &stride,
                                               &offset);
        m_deferred_context->IASetInputLayout(input_layout);
        m_deferred_context->VSSetShader(
            dx11_vertex_shader->directx_vertex_shader(),
            nullptr,
            0);
        m_deferred_context->PSSetShader(
            dx11_pixel_shader->directx_pixel_shader(),
            nullptr,
            0);

        m_deferred_context->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_deferred_context->Draw(
            static_cast<UINT>(command.vertices()->element_count()),
            0);
    }

    void command_list::execute()
    {
        if (!m_command_list) {
            ID3D11CommandList* command_list = nullptr;
            auto               hr =
                m_deferred_context->FinishCommandList(FALSE, &command_list);
            CHECK_HRESULT(hr, ID3D11DeviceContext, FinishCommandList);
            m_command_list.reset(command_list);
        }
        m_dx11_context.device_context()->ExecuteCommandList(
            m_command_list.get(),
            FALSE);
    }

} // namespace mge::dx11

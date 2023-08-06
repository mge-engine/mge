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
            offset += element.binary_size();
            */
        }
    }
#endif

    void command_list::draw(const mge::draw_command& command)
    {
#if 0
        //[[maybe_unused]] auto dx11_program =
        //    static_cast<mge::dx11::program*>(command.program().get());
        const auto&               layout = command.vertices()->layout();
        const size_t              layout_size = layout.size();
        D3D11_INPUT_ELEMENT_DESC* input_layout =
            static_cast<D3D11_INPUT_ELEMENT_DESC*>(
                alloca(layout_size * sizeof(D3D11_INPUT_ELEMENT_DESC)));
        fill_input_layout(layout, input_layout);
#endif
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

// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "render_context.hpp"
#include "error.hpp"

namespace dx11 {
    command_list::command_list(render_context& context)
        : mge::command_list(context)
    {
        ID3D11DeviceContext *deferred_context;
        auto hr = context.device()->CreateDeferredContext(0, &deferred_context);
        CHECK_HRESULT(hr, ID3D11Device, CreateDeferredContext);
        m_deferred_context.reset(deferred_context);
    }

    void
    command_list::clear(const mge::rgba_color& c)
    {
        assert_not_immutable();

        float clearcolor[4] = { c.r, c.g, c.b, c.a };
        m_deferred_context->ClearRenderTargetView(dx11_render_target_view(*m_context),
                                                  clearcolor);
    }

    void
    command_list::draw(const mge::draw_command& cmd)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    command_list::draw(mge::draw_command&& cmd)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    command_list::clear_depth(float value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
        //m_deferred_context->ClearDepthStencilView
    }

    void
    command_list::clear_stencil(int32_t value)
    {
        MGE_THROW_NOT_IMPLEMENTED;
        //m_deferred_context->ClearDepthStencilView
    }

    void
    command_list::on_finish()
    {
        ID3D11CommandList *command_list;
        auto hr = m_deferred_context->FinishCommandList(FALSE, &command_list);
        CHECK_HRESULT(hr, ID3D11DeviceContext, FinishCommandList);
        m_command_list.reset(command_list);
        //m_deferred_context.reset();
    }

    void
    command_list::on_play(const mge::command_list_ref& commands)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    command_list::on_clear()
    {
        ID3D11CommandList *command_list;
        auto hr = m_deferred_context->FinishCommandList(TRUE, &command_list);
        CHECK_HRESULT(hr, ID3D11DeviceContext, FinishCommandList);
        m_command_list.reset();
        command_list->Release();
    }
}

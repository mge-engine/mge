#include "command_list.hpp"
#include "error.hpp"

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

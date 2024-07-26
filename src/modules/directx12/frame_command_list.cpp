#include "frame_command_list.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::dx12 {
    frame_command_list::frame_command_list(render_context& context,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_dx12_context(context)
    {
        m_command_allocator = m_dx12_context.get_command_allocator();
        auto hr = m_dx12_context.device()->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            m_command_allocator.Get(),
            nullptr,
            IID_PPV_ARGS(m_command_list.GetAddressOf()));
        CHECK_HRESULT(hr, ID3D12Device, CreateCommandList);
        m_command_list->Close();
    }

    frame_command_list::~frame_command_list()
    {
        m_dx12_context.release_command_allocator(m_command_allocator);
    }

    void frame_command_list::clear(const mge::rgba_color& c) {}

    void frame_command_list::draw(const mge::draw_command& command) {}

    void frame_command_list::execute() {}

    void frame_command_list::finish() {}

} // namespace mge::dx12
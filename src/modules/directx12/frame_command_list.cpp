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
        auto rc = m_command_allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc = m_command_list->Reset(m_command_allocator.Get(), nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);

        // assign to backbuffer
        D3D12_RESOURCE_BARRIER barrier = {
            .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
            .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
            .Transition =
                {.pResource = m_dx12_context.backbuffer(backbuffer_index).Get(),
                 .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                 .StateBefore = D3D12_RESOURCE_STATE_PRESENT,
                 .StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET},
        }; // namespace mge::dx12
        m_command_list->ResourceBarrier(1, &barrier);
        m_command_list->RSSetViewports(1, &m_dx12_context.viewport());
        m_command_list->RSSetScissorRects(1, &m_dx12_context.scissor_rect());
        auto rtv_handle = m_dx12_context.rtv_handle(backbuffer_index);
        m_command_list->OMSetRenderTargets(1, &rtv_handle, FALSE, nullptr);
    }

    frame_command_list::~frame_command_list()
    {
        m_dx12_context.release_command_allocator(m_command_allocator);
    }

    void frame_command_list::clear(const mge::rgba_color& c)
    {
        auto rtv_handle = m_dx12_context.rtv_handle(backbuffer_index());
        m_command_list->ClearRenderTargetView(rtv_handle, c.data(), 0, nullptr);
    }

    void frame_command_list::draw(const mge::draw_command& command) {}

    void frame_command_list::execute()
    {
        ID3D12CommandList* lists[] = {m_command_list.Get()};
        // TODO: multiple command list submits before waiting
        m_dx12_context.command_queue()->ExecuteCommandLists(1, lists);
        m_dx12_context.wait_for_command_queue();
        // TODO: reset command list, but no synchronous wait here
        auto rc = m_command_allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc = m_command_list->Reset(m_command_allocator.Get(), nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
    }

    void frame_command_list::finish()
    {
        D3D12_RESOURCE_BARRIER barrier = {
            .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
            .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
            .Transition = {
                .pResource =
                    m_dx12_context.backbuffer(backbuffer_index()).Get(),
                .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                .StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
                .StateAfter = D3D12_RESOURCE_STATE_PRESENT}};
        m_command_list->ResourceBarrier(1, &barrier);
        m_command_list->Close();
    }

} // namespace mge::dx12
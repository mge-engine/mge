// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_system.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {

    render_context::render_context(mge::dx12::render_system& render_system_,
                                   mge::dx12::window&        window_)
        : render_context_base(render_system_, window_.extent())
        , m_window(window_)
    {
        m_rtv_next_index = buffer_count;
        m_dsv_next_index = buffer_count;

        auto fd = m_render_system.frame_debugger();
        if (fd) {
            fd->set_context(frame_debugger::capture_context{m_device.Get(),
                                                            m_window.hwnd()});
        }
    }

    render_context::~render_context()
    {
        if (m_render_system.frame_debugger()) {
            auto fd = m_render_system.frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(DX12, "Ending frame recording");
                fd->end_capture();
            }
        }
    }

    void render_context::initialize()
    {
        MGE_DEBUG_TRACE(DX12, "Create swap chain");
        create_swap_chain();
        MGE_DEBUG_TRACE(DX12, "Create descriptor heap");
        create_descriptor_heap();
        MGE_DEBUG_TRACE(DX12, "Update render target views");
        update_render_target_views();
        MGE_DEBUG_TRACE(DX12, "Create depth stencil views");
        create_depth_stencil_views();
        MGE_DEBUG_TRACE(DX12, "Create direct command lists");
        create_command_lists();
    }

    void render_context::create_swap_chain()
    {
        MGE_DEBUG_TRACE(DX12, "Create swap chain");
        DXGI_SWAP_CHAIN_DESC1 swap_chain_desc      = {};
        swap_chain_desc.Width                       = m_window.extent().width;
        swap_chain_desc.Height                      = m_window.extent().height;
        swap_chain_desc.Format                      = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.Stereo                      = FALSE;
        swap_chain_desc.SampleDesc.Count            = 1;
        swap_chain_desc.SampleDesc.Quality          = 0;
        swap_chain_desc.BufferUsage                 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount                 = buffer_count;
        swap_chain_desc.Scaling                     = DXGI_SCALING_STRETCH;
        swap_chain_desc.SwapEffect                  = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swap_chain_desc.AlphaMode                   = DXGI_ALPHA_MODE_UNSPECIFIED;
        swap_chain_desc.Flags                       = 0;

        mge::com_ptr<IDXGISwapChain1> swap_chain1;
        auto rc = m_factory->CreateSwapChainForHwnd(m_command_queue.Get(),
                                                    m_window.hwnd(),
                                                    &swap_chain_desc,
                                                    nullptr,
                                                    nullptr,
                                                    &swap_chain1);
        CHECK_HRESULT(rc, IDXGIFactory4, CreateSwapChainForHwnd);

        rc = m_factory->MakeWindowAssociation(m_window.hwnd(),
                                              DXGI_MWA_NO_ALT_ENTER);
        CHECK_HRESULT(rc, IDXGIFactory4, MakeWindowAssociation);

        rc = swap_chain1.As(&m_swap_chain);
        CHECK_HRESULT(rc, com_ptr, As<IDXGISwapChain4>);
    }

    void render_context::update_render_target_views()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle =
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart();

        for (int i = 0; i < buffer_count; ++i) {
            MGE_DEBUG_TRACE(
                DX12,
                "Create render target view for back buffer #{} of swap chain",
                i);
            mge::com_ptr<ID3D12Resource> backbuffer;
            auto rc = m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&backbuffer));
            CHECK_HRESULT(rc, IDXGISwapChain4, GetBuffer);
            std::wstringstream ws;
            ws << "mge::dx12::render_context::backbuffer#" << i;
            backbuffer->SetName(ws.str().c_str());
            m_device->CreateRenderTargetView(backbuffer.Get(),
                                             nullptr,
                                             rtv_handle);
            m_backbuffers.emplace_back(backbuffer);
            rtv_handle.ptr += m_rtv_descriptor_size;
        }
    }

    void render_context::create_depth_stencil_views()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle =
            m_dsv_heap->GetCPUDescriptorHandleForHeapStart();

        for (uint32_t i = 0; i < buffer_count; ++i) {
            D3D12_RESOURCE_DESC depth_desc          = {};
            depth_desc.Dimension                    = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            depth_desc.Alignment                    = 0;
            depth_desc.Width                        = m_window.extent().width;
            depth_desc.Height                       = m_window.extent().height;
            depth_desc.DepthOrArraySize             = 1;
            depth_desc.MipLevels                    = 1;
            depth_desc.Format                       = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depth_desc.SampleDesc.Count             = 1;
            depth_desc.SampleDesc.Quality           = 0;
            depth_desc.Layout                       = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            depth_desc.Flags                        = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

            D3D12_CLEAR_VALUE clear_value           = {};
            clear_value.Format                      = DXGI_FORMAT_D24_UNORM_S8_UINT;
            clear_value.DepthStencil.Depth          = 1.0f;
            clear_value.DepthStencil.Stencil        = 0;

            D3D12_HEAP_PROPERTIES heap_props        = {};
            heap_props.Type                         = D3D12_HEAP_TYPE_DEFAULT;
            heap_props.CPUPageProperty              = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heap_props.MemoryPoolPreference         = D3D12_MEMORY_POOL_UNKNOWN;
            heap_props.CreationNodeMask             = 1;
            heap_props.VisibleNodeMask              = 1;

            mge::com_ptr<ID3D12Resource> depth_buffer;
            auto rc = m_device->CreateCommittedResource(
                &heap_props,
                D3D12_HEAP_FLAG_NONE,
                &depth_desc,
                D3D12_RESOURCE_STATE_DEPTH_WRITE,
                &clear_value,
                IID_PPV_ARGS(&depth_buffer));
            CHECK_HRESULT(rc, ID3D12Device, CreateCommittedResource);

            std::wstringstream ws;
            ws << "mge::dx12::render_context::depth_buffer#" << i;
            depth_buffer->SetName(ws.str().c_str());

            D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc  = {};
            dsv_desc.Format                         = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dsv_desc.ViewDimension                  = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsv_desc.Flags                          = D3D12_DSV_FLAG_NONE;

            m_device->CreateDepthStencilView(depth_buffer.Get(),
                                             &dsv_desc,
                                             dsv_handle);
            m_depth_buffers.emplace_back(depth_buffer);
            dsv_handle.ptr += m_dsv_descriptor_size;
        }
    }

    void render_context::setup_backbuffer_as_render_target(
        ID3D12GraphicsCommandList*   cmd_list,
        D3D12_CPU_DESCRIPTOR_HANDLE& pass_rtv,
        D3D12_CPU_DESCRIPTOR_HANDLE& pass_dsv,
        bool&                        pass_has_dsv,
        DXGI_FORMAT&                 pass_rtv_format,
        DXGI_FORMAT&                 pass_dsv_format)
    {
        uint32_t current_buffer_index =
            m_swap_chain->GetCurrentBackBufferIndex();

        if (m_draw_state == draw_state::NONE) {
            wait_for_command_queue();
            auto rc = m_command_allocator->Reset();
            CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
            rc = m_command_list->Reset(m_command_allocator.Get(), nullptr);
            CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
        }

        if (m_draw_state != draw_state::DRAW) {
            D3D12_RESOURCE_BARRIER present_to_render = {
                .Type  = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
                .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
                .Transition =
                    {.pResource =
                         m_backbuffers[current_buffer_index].Get(),
                     .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                     .StateBefore = D3D12_RESOURCE_STATE_PRESENT,
                     .StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET},
            };
            cmd_list->ResourceBarrier(1, &present_to_render);
            m_draw_state = draw_state::DRAW;
        }

        pass_rtv        = rtv_handle(current_buffer_index);
        pass_dsv        = dsv_handle(current_buffer_index);
        pass_has_dsv    = true;
        pass_rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        pass_dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        cmd_list->OMSetRenderTargets(1, &pass_rtv, FALSE, &pass_dsv);
    }

    void render_context::on_frame_present()
    {
        if (m_draw_state == draw_state::DRAW) {
            D3D12_RESOURCE_BARRIER render_to_present = {
                .Type  = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
                .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
                .Transition = {
                    .pResource =
                        m_backbuffers[m_swap_chain->GetCurrentBackBufferIndex()]
                            .Get(),
                    .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                    .StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
                    .StateAfter  = D3D12_RESOURCE_STATE_PRESENT}};
            m_command_list->ResourceBarrier(1, &render_to_present);
        }

        if (m_draw_state == draw_state::DRAW ||
            m_draw_state == draw_state::FBO_DRAW) {
            m_command_list->Close();
            ID3D12CommandList* lists[] = {m_command_list.Get()};
            m_command_queue->ExecuteCommandLists(1, lists);
        }

        m_draw_state = draw_state::SUBMIT;
        m_swap_chain->Present(0, 0);
        m_draw_state = draw_state::NONE;
    }

    mge::image_ref render_context::screenshot()
    {
        auto  backbuffer_index = m_swap_chain->GetCurrentBackBufferIndex();
        auto* backbuffer       = m_backbuffers[backbuffer_index].Get();
        auto  bb_desc          = backbuffer->GetDesc();

        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
        uint32_t                           num_rows;
        uint64_t                           row_size;
        uint64_t                           total_size;
        m_device->GetCopyableFootprints(&bb_desc,
                                        0,
                                        1,
                                        0,
                                        &footprint,
                                        &num_rows,
                                        &row_size,
                                        &total_size);

        D3D12_HEAP_PROPERTIES readback_heap    = {};
        readback_heap.Type                      = D3D12_HEAP_TYPE_READBACK;

        D3D12_RESOURCE_DESC readback_desc       = {};
        readback_desc.Dimension                 = D3D12_RESOURCE_DIMENSION_BUFFER;
        readback_desc.Width                     = total_size;
        readback_desc.Height                    = 1;
        readback_desc.DepthOrArraySize          = 1;
        readback_desc.MipLevels                 = 1;
        readback_desc.SampleDesc.Count          = 1;
        readback_desc.Layout                    = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        mge::com_ptr<ID3D12Resource> readback_buffer;
        auto rc = m_device->CreateCommittedResource(&readback_heap,
                                                    D3D12_HEAP_FLAG_NONE,
                                                    &readback_desc,
                                                    D3D12_RESOURCE_STATE_COPY_DEST,
                                                    nullptr,
                                                    IID_PPV_ARGS(&readback_buffer));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommittedResource);

        {
            std::lock_guard<mge::mutex> lock(m_data_lock);

            D3D12_RESOURCE_BARRIER to_copy_src          = {};
            to_copy_src.Type                            = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            to_copy_src.Transition.pResource            = backbuffer;
            to_copy_src.Transition.StateBefore          = D3D12_RESOURCE_STATE_RENDER_TARGET;
            to_copy_src.Transition.StateAfter           = D3D12_RESOURCE_STATE_COPY_SOURCE;
            to_copy_src.Transition.Subresource          = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            m_xfer_command_list->ResourceBarrier(1, &to_copy_src);

            D3D12_TEXTURE_COPY_LOCATION dst_loc         = {};
            dst_loc.pResource                           = readback_buffer.Get();
            dst_loc.Type                                = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            dst_loc.PlacedFootprint                     = footprint;

            D3D12_TEXTURE_COPY_LOCATION src_loc         = {};
            src_loc.pResource                           = backbuffer;
            src_loc.Type                                = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            src_loc.SubresourceIndex                    = 0;

            m_xfer_command_list->CopyTextureRegion(&dst_loc,
                                                   0, 0, 0,
                                                   &src_loc,
                                                   nullptr);

            D3D12_RESOURCE_BARRIER to_render_target     = {};
            to_render_target.Type                       = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            to_render_target.Transition.pResource       = backbuffer;
            to_render_target.Transition.StateBefore     = D3D12_RESOURCE_STATE_COPY_SOURCE;
            to_render_target.Transition.StateAfter      = D3D12_RESOURCE_STATE_RENDER_TARGET;
            to_render_target.Transition.Subresource     = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            m_xfer_command_list->ResourceBarrier(1, &to_render_target);

            m_xfer_command_list->Close();
            ID3D12CommandList* lists[] = {m_xfer_command_list.Get()};
            m_command_queue->ExecuteCommandLists(1, lists);
            wait_for_command_queue();

            rc = m_xfer_command_allocator->Reset();
            CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
            rc = m_xfer_command_list->Reset(m_xfer_command_allocator.Get(),
                                            nullptr);
            CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
        }

        void*       mapped     = nullptr;
        D3D12_RANGE read_range = {0, static_cast<SIZE_T>(total_size)};
        rc = readback_buffer->Map(0, &read_range, &mapped);
        CHECK_HRESULT(rc, ID3D12Resource, Map);

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        uint32_t          bb_width  = static_cast<uint32_t>(bb_desc.Width);
        uint32_t          bb_height = static_cast<uint32_t>(bb_desc.Height);
        auto              img       = std::make_shared<mge::memory_image>(
            fmt, mge::extent(bb_width, bb_height));
        uint32_t    pixel_row_size = bb_width * 4;
        auto*       dst            = static_cast<uint8_t*>(img->data());
        const auto* src            = static_cast<const uint8_t*>(mapped);
        for (uint32_t y = 0; y < bb_height; ++y) {
            memcpy(dst + y * pixel_row_size,
                   src + y * footprint.Footprint.RowPitch,
                   pixel_row_size);
        }

        D3D12_RANGE written_range = {0, 0};
        readback_buffer->Unmap(0, &written_range);
        return img;
    }

} // namespace mge::dx12

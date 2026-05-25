// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "headless_render_context.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_system.hpp"
#include "texture.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {

    headless_render_context::headless_render_context(
        mge::dx12::render_system& render_system_,
        const mge::extent&        extent_)
        : render_context_base(render_system_, extent_)
    {
        MGE_DEBUG_TRACE(DX12, "Create headless DX12 render context");
        create_descriptor_heap();
        create_command_lists();
    }

    void headless_render_context::setup_backbuffer_as_render_target(
        ID3D12GraphicsCommandList* /*cmd_list*/,
        D3D12_CPU_DESCRIPTOR_HANDLE& /*pass_rtv*/,
        D3D12_CPU_DESCRIPTOR_HANDLE& /*pass_dsv*/,
        bool& /*pass_has_dsv*/,
        DXGI_FORMAT& /*pass_rtv_format*/,
        DXGI_FORMAT& /*pass_dsv_format*/)
    {
        MGE_THROW(mge::illegal_state)
            << "Headless DX12 context cannot render without a frame buffer";
    }

    void headless_render_context::on_frame_present()
    {
        if (m_draw_state == draw_state::FBO_DRAW) {
            m_command_list->Close();
            ID3D12CommandList* lists[] = {m_command_list.Get()};
            m_command_queue->ExecuteCommandLists(1, lists);
            wait_for_command_queue();
        }
        m_draw_state = draw_state::NONE;
    }

    mge::image_ref headless_render_context::screenshot()
    {
        auto& p  = pass(0);
        auto* fb = static_cast<dx12::frame_buffer*>(p.frame_buffer().get());
        if (!fb) {
            MGE_THROW(mge::illegal_state)
                << "Headless screenshot: pass 0 has no frame buffer";
        }

        auto color_tex = fb->color_attachment(0);
        if (!color_tex) {
            MGE_THROW(mge::illegal_state)
                << "Headless screenshot: frame buffer has no color attachment";
        }

        auto* dx12_tex        = static_cast<dx12::texture*>(color_tex.get());
        auto* color_resource  = dx12_tex->resource();
        auto  tex_desc        = color_resource->GetDesc();

        D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
        uint32_t                           num_rows;
        uint64_t                           row_size;
        uint64_t                           total_size;
        m_device->GetCopyableFootprints(&tex_desc,
                                        0,
                                        1,
                                        0,
                                        &footprint,
                                        &num_rows,
                                        &row_size,
                                        &total_size);

        D3D12_HEAP_PROPERTIES readback_heap   = {};
        readback_heap.Type                     = D3D12_HEAP_TYPE_READBACK;

        D3D12_RESOURCE_DESC readback_desc      = {};
        readback_desc.Dimension                = D3D12_RESOURCE_DIMENSION_BUFFER;
        readback_desc.Width                    = total_size;
        readback_desc.Height                   = 1;
        readback_desc.DepthOrArraySize         = 1;
        readback_desc.MipLevels                = 1;
        readback_desc.SampleDesc.Count         = 1;
        readback_desc.Layout                   = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

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

            D3D12_RESOURCE_BARRIER to_copy_src      = {};
            to_copy_src.Type                        = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            to_copy_src.Transition.pResource        = color_resource;
            to_copy_src.Transition.StateBefore      = D3D12_RESOURCE_STATE_RENDER_TARGET;
            to_copy_src.Transition.StateAfter       = D3D12_RESOURCE_STATE_COPY_SOURCE;
            to_copy_src.Transition.Subresource      = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            m_xfer_command_list->ResourceBarrier(1, &to_copy_src);

            D3D12_TEXTURE_COPY_LOCATION dst_loc     = {};
            dst_loc.pResource                       = readback_buffer.Get();
            dst_loc.Type                            = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            dst_loc.PlacedFootprint                 = footprint;

            D3D12_TEXTURE_COPY_LOCATION src_loc     = {};
            src_loc.pResource                       = color_resource;
            src_loc.Type                            = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            src_loc.SubresourceIndex                = 0;

            m_xfer_command_list->CopyTextureRegion(&dst_loc,
                                                   0, 0, 0,
                                                   &src_loc,
                                                   nullptr);

            D3D12_RESOURCE_BARRIER to_render_target    = {};
            to_render_target.Type                      = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            to_render_target.Transition.pResource      = color_resource;
            to_render_target.Transition.StateBefore    = D3D12_RESOURCE_STATE_COPY_SOURCE;
            to_render_target.Transition.StateAfter     = D3D12_RESOURCE_STATE_RENDER_TARGET;
            to_render_target.Transition.Subresource    = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
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
        uint32_t tex_width  = static_cast<uint32_t>(tex_desc.Width);
        uint32_t tex_height = static_cast<uint32_t>(tex_desc.Height);
        auto     img        = std::make_shared<mge::memory_image>(
            fmt, mge::extent(tex_width, tex_height));
        uint32_t    pixel_row_size = tex_width * 4;
        auto*       dst            = static_cast<uint8_t*>(img->data());
        const auto* src            = static_cast<const uint8_t*>(mapped);
        for (uint32_t y = 0; y < tex_height; ++y) {
            memcpy(dst + y * pixel_row_size,
                   src + y * footprint.Footprint.RowPitch,
                   pixel_row_size);
        }

        D3D12_RANGE written_range = {0, 0};
        readback_buffer->Unmap(0, &written_range);
        return img;
    }

} // namespace mge::dx12

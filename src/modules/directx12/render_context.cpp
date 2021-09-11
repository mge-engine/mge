// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/trace.hpp"
#include "mge/win32/com_ptr.hpp"
#include "render_system.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    render_context::render_context(mge::dx12::render_system& render_system_,
                                   mge::dx12::window&        window_)
        : m_render_system(render_system_)
        , m_window(window_)
    {
        create_factory();
        create_adapter();
        create_device();
        enable_debug_messages();
        create_command_queue();
        m_swap_chain =
            std::make_shared<mge::dx12::swap_chain>(render_system_, *this);
        create_descriptor_heap()
    }

    void render_context::create_command_queue()
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;
        auto rc =
            m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);
    }

    void render_context::create_device()
    {
        auto rc = D3D12CreateDevice(m_adapter.Get(),
                                    D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(&m_device));
        CHECK_HRESULT(rc, , D3D12CreateDevice);
    }

    void render_context::enable_debug_messages()
    {
        if (m_render_system.debug()) {
            mge::com_ptr<ID3D12InfoQueue> infoqueue;
            if (SUCCEEDED(m_device.As(&infoqueue))) {
                infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION,
                                              TRUE);
                infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,
                                              TRUE);
                infoqueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,
                                              TRUE);

                D3D12_MESSAGE_SEVERITY denied_severities[] = {
                    D3D12_MESSAGE_SEVERITY_INFO};

                D3D12_MESSAGE_ID denied_message_ids[] = {
                    // clear render target with different color
                    D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
                    // both issued in debugger
                    D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
                    D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
                };

                D3D12_INFO_QUEUE_FILTER deny_filter = {};
                deny_filter.DenyList.NumSeverities =
                    checked_cast<UINT>(array_size(denied_severities));
                deny_filter.DenyList.pSeverityList = denied_severities;
                deny_filter.DenyList.NumIDs =
                    checked_cast<UINT>(array_size(denied_message_ids));
                deny_filter.DenyList.pIDList = denied_message_ids;
                auto rc = infoqueue->PushStorageFilter(&deny_filter);
                CHECK_HRESULT(rc, ID3D12InfoQueue, PushStorageFilter);
            }
        }
    }

    void render_context::create_factory()
    {
        UINT factory_flags = 0;
        if (m_render_system.debug()) {
            factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        MGE_DEBUG_TRACE(DX12) << "Create DXGI Factory";
        auto rc = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory));
        CHECK_HRESULT(rc, , CreateDXGIFactory2);
    }

    void render_context::create_adapter()
    {
        HRESULT rc = S_OK;
        if (m_render_system.warp()) {
            mge::com_ptr<IDXGIAdapter1> adapter1;
            rc = m_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter1));
            CHECK_HRESULT(rc, IDXGIFactory4, EnumWarpAdapter);
            rc = adapter1.As(&m_adapter);
            CHECK_HRESULT(rc, com_ptr, As<IDXGIAdapter4>);
        } else {
            size_t max_dedicated_video_mem = 0;

            mge::com_ptr<IDXGIAdapter1> adapter1;
            for (uint32_t i = 0;
                 m_factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND;
                 ++i) {
                DXGI_ADAPTER_DESC1 desc_adapter1;

                rc = adapter1->GetDesc1(&desc_adapter1);
                CHECK_HRESULT(rc, IDXGIAdapter1, GetDesc1);

                if ((desc_adapter1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0) {
                    if (SUCCEEDED(D3D12CreateDevice(adapter1.Get(),
                                                    D3D_FEATURE_LEVEL_11_0,
                                                    __uuidof(ID3D12Device),
                                                    nullptr))) {
                        if (desc_adapter1.DedicatedVideoMemory >
                            max_dedicated_video_mem) {
                            max_dedicated_video_mem =
                                desc_adapter1.DedicatedVideoMemory;
                            rc = adapter1.As(&m_adapter);
                            CHECK_HRESULT(rc, com_ptr, As<IDXGIAdapter4>);
                        }
                    }
                }
            }

            if (!m_adapter.Get()) {
                MGE_THROW(dx12::error) << "No suitable hardware adapter found";
            }
        }
    }

    void render_context::create_descriptor_heap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};

        rtv_heap_desc.NumDescriptors = buffer_count;
        rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        auto rc = m_device->CreateDescriptorHeap(&rtv_heap_desc,
                                                 IID_PPV_ARGS(&m_rtv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    void render_context::update_render_target_views()
    {
        D3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart());

        for (int i = 0; i < buffer_count; ++i) {
            mge::com_ptr<ID3D12Resource> backbuffer;
#if 0
            ThrowIfFailed(swa->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

            device->CreateRenderTargetView(backBuffer.Get(),
                                           nullptr,
                                           rtvHandle);

            g_BackBuffers[i] = backBuffer;

            rtvHandle.Offset(rtvDescriptorSize);
#endif
        }
    }

    void render_context::initialize() {}

    render_context::~render_context() {}

    mge::index_buffer_ref render_context::create_index_buffer(mge::data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref ref;
        return ref;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref ref;
        return ref;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref ref;
        return ref;
    }

} // namespace mge::dx12
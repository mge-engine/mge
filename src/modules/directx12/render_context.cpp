// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "window.hpp"
#include "error.hpp"
#include "common.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/to_utf8.hpp"
#include "mge/core/system_error.hpp"
#include "mge/graphics/memory_command_list.hpp"

MGE_USE_LOG(DX12);

namespace dx12 {
    render_context::render_context(window *w, const system_config& config)
        : mge::render_context(w)
        ,m_window(w)
        ,m_event(0)
        ,m_feature_level((D3D_FEATURE_LEVEL) 0)
        ,m_frame_index(0)
        ,m_rtv_descriptor_size(0)
        ,m_fence_value(0)
    {
        init_context(config);
    }

    render_context::~render_context()
    {
        if (m_event) {
            CloseHandle(m_event);
        }
    }

    void
    render_context::flush()
    {
        return;
    }

    void
    render_context::create_factory()
    {
        MGE_DEBUG_LOG(DX12) << "Create DXGI factory";
        IDXGIFactory4 *factory = nullptr;
        HRESULT rc = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void **) &factory);
        CHECK_HRESULT(rc, , CreateDXGIFactory1);
        if(!factory) {
            MGE_THROW(mge::null_pointer);
        }
        m_dxgi_factory.reset(factory);
    }

    void
    render_context::select_adapter()
    {
        MGE_DEBUG_LOG(DX12) << "Selecting adapter";
        IDXGIAdapter3 *adapter = nullptr;
        uint32_t index = 0;
        while(m_dxgi_factory->EnumAdapters(index, (IDXGIAdapter **)&adapter) == S_OK) {
            MGE_DEBUG_LOG(DX12) << "Adapter " << index;
            DXGI_ADAPTER_DESC1 desc;
            HRESULT rc = adapter->GetDesc1(&desc);
            if(SUCCEEDED(rc)) {
                MGE_DEBUG_LOG(DX12) << "Description: " << mge::to_utf8(&desc.Description[0]);
                MGE_DEBUG_LOG(DX12) << "Vendor: " << std::hex << desc.VendorId << std::dec;
            }
            m_adapter.reset(adapter);
            if((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0) {
                if(setup_device()) {
                    break;
                }
            } else {
                MGE_DEBUG_LOG(DX12) << "Skip software driver";
                m_adapter.reset();
            }
            ++index;
        }
    }

    bool
    render_context::setup_device()
    {
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_12_1,
                                              D3D_FEATURE_LEVEL_12_0,
                                              D3D_FEATURE_LEVEL_11_1,
                                              D3D_FEATURE_LEVEL_11_0, };
        ID3D12Device *device;
        HRESULT hr = 0;
        for(const auto level : featurelevels) {
            hr = D3D12CreateDevice(m_adapter.get(),
                                   level,
                                   IID_ID3D12Device,
                                   (void **)&device);
            if (SUCCEEDED(hr)) {
                MGE_DEBUG_LOG(DX12) << "Using feature level " << level;
                m_feature_level = level;
                m_device.reset(device);
                return true;
            } 
        }

        return false;
    }

    void
    render_context::create_command_queue()
    {
        MGE_DEBUG_LOG(DX12) << "Create command queue";
        ID3D12CommandQueue *command_queue = nullptr;
        D3D12_COMMAND_QUEUE_DESC qdesc = {};
        qdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        qdesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        qdesc.NodeMask = 0;
        HRESULT rc = m_device->CreateCommandQueue(&qdesc, IID_PPV_ARGS(&command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);
        m_command_queue.reset(command_queue);
        MGE_DEBUG_LOG(DX12) << "Command queue is " << (void *)m_command_queue.get();
    }

    void
    render_context::create_swap_chain()
    {
        MGE_DEBUG_LOG(DX12) << "Create swap chain";
        DXGI_SWAP_CHAIN_DESC1 swdesc = {};
        swdesc.Width = m_window->rect().width();
        swdesc.Height = m_window->rect().height();
        swdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swdesc.Stereo = FALSE;
        swdesc.SampleDesc = { 1, 0 }; // TODO: multisampling
        swdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swdesc.BufferCount = RENDER_TARGET_COUNT;
        swdesc.Scaling = DXGI_SCALING_STRETCH;
        swdesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swdesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swdesc.Flags = 0; // TODO: DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
        IDXGISwapChain4 *swap_chain = nullptr;
        HRESULT rc = m_dxgi_factory->CreateSwapChainForHwnd(m_command_queue.get(),
                                                            m_window->hwnd(),
                                                            &swdesc,
                                                            nullptr,
                                                            nullptr,
                                                            (IDXGISwapChain1**)&swap_chain);
        CHECK_HRESULT(rc, IDXGIFactory, CreateSwapChainForHwnd);
        m_swap_chain.reset(swap_chain);
        m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    }

    void
    render_context::disable_fullscreen_transition()
    {
        auto rc = m_dxgi_factory->MakeWindowAssociation(m_window->hwnd(),
                                                        DXGI_MWA_NO_ALT_ENTER);
        CHECK_HRESULT(rc, IDXGIFactory4, MakeWindowAssociation);
    }

    void
    render_context::create_descriptor_heap()
    {
        MGE_DEBUG_LOG(DX12) << "Created descriptor heap for render target views";
        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        ID3D12DescriptorHeap *rtv_heap;
        heap_desc.NumDescriptors = RENDER_TARGET_COUNT;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        auto rc = m_device->CreateDescriptorHeap(&heap_desc,
                                                 IID_PPV_ARGS(&rtv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_rtv_heap.reset(rtv_heap);
        m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    void
    render_context::create_render_target_views()
    {
        MGE_DEBUG_LOG(DX12) << "Create render target views";
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());

        for (uint32_t n = 0; n < RENDER_TARGET_COUNT; n++) {
            ID3D12Resource *rt;
            auto rc = m_swap_chain->GetBuffer(n, IID_PPV_ARGS(&rt));
            CHECK_HRESULT(rc, IDXGISwapChain, GetBuffer);
            m_buffers[n].reset(rt);
            m_device->CreateRenderTargetView(m_buffers[n].get(), nullptr, rtv_handle);
            rtv_handle.Offset(m_rtv_descriptor_size);
         }
    }

    void
    render_context::create_command_allocator()
    {
        MGE_DEBUG_LOG(DX12) << "Create command allocator";
        ID3D12CommandAllocator *command_alloc = nullptr;
        auto rc = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_alloc));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);
        m_command_allocator.reset(command_alloc);
    }

    void
    render_context::create_fence()
    {
        MGE_DEBUG_LOG(DX12) << "Create fence and fence event";
        ID3D12Fence *fence = nullptr;
        auto rc = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
        CHECK_HRESULT(rc, ID3D12Device, CreateFence);
        m_fence.reset(fence);

        m_event = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
        if(!m_event) {
            MGE_THROW_SYSCALL_FAILED(CreateEventEx);
        }
    }

    void
    render_context::init_context(const system_config& config)
    {
        create_factory();
        select_adapter();
        create_command_queue();
        create_swap_chain();
        disable_fullscreen_transition();
        create_descriptor_heap();
        create_render_target_views();
        create_command_allocator();
        create_fence();
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type,
                                        mge::usage usage,
                                        size_t element_count,
                                        void *initial_data)
    {
        mge::index_buffer_ref result;
        return result;
    }

    mge::vertex_buffer_ref 
    render_context::create_vertex_buffer(const mge::vertex_layout& layout,
                                         mge::usage usage,
                                         size_t element_count,
                                         void *initial_data)
    {
        mge::vertex_buffer_ref result;
        return result;
    }

    mge::texture_2d_ref 
    render_context::create_texture_2d(const mge::image_ref& image)
    {
        mge::texture_2d_ref result;
        return result;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d()
    {
        mge::texture_2d_ref result;
        return result;
    }

    void
    render_context::shader_languages(std::vector<mge::shader_language>& languages) const
    {
        // Shader model did not change anymore since DirectX11 and stays at 5.1
        // https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-devices-downlevel-intro
        languages.push_back(mge::shader_language("HLSL", mge::version(5,1)));
    }

    mge::shader_ref
    render_context::create_shader(mge::shader_type type)
    {
        mge::shader_ref result;
        return result;
    }

    mge::pipeline_ref
    render_context::create_pipeline()
    {
        mge::pipeline_ref result;
        return result;
    }

    mge::command_list_ref
    render_context::create_command_list()
    {
        return std::make_shared<mge::memory_command_list>(*this);
    }

    void
    render_context::execute(const mge::command_list_ref& commands)
    {}

}

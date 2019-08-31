// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "window.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/to_utf8.hpp"

MGE_USE_LOG(DX12);

namespace dx12 {
    render_context::render_context(window *w, const system_config& config)
        : mge::render_context(w)
        ,m_window(w)
        ,m_feature_level((D3D_FEATURE_LEVEL) 0)
        ,m_frame_index(0)
        ,m_rtv_descriptor_size(0)
    {
        init_context(config);
    }

    render_context::~render_context()
    {}

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
        qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        qdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        HRESULT rc = m_device->CreateCommandQueue(&qdesc, IID_PPV_ARGS(&command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);
        m_command_queue.reset(command_queue);
    }

    void
    render_context::create_swap_chain()
    {
        MGE_DEBUG_LOG(DX12) << "Create swap chain";

        DXGI_SWAP_CHAIN_DESC1 swdesc = {};
        swdesc.BufferCount = RENDER_TARGET_COUNT;
        swdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swdesc.SampleDesc.Count = 1; // TODO: multisampling
        swdesc.SwapEffect = (DXGI_SWAP_EFFECT)4; // DXGI_SWAP_EFFECT_FLIP_DISCARD;
        IDXGISwapChain3 *swap_chain = nullptr;
        HRESULT rc = m_dxgi_factory->CreateSwapChainForHwnd(m_command_queue.get(),
                                                            m_window->hwnd(),
                                                            &swdesc,
                                                            nullptr,
                                                            nullptr,
                                                            (IDXGISwapChain1**)&swap_chain);
        CHECK_HRESULT(rc, IDXGIFactory, CreateSwapChain);
        m_swap_chain.reset(swap_chain);
        // fullscreen transition disable
        rc = m_dxgi_factory->MakeWindowAssociation(m_window->hwnd(),
                                                   DXGI_MWA_NO_ALT_ENTER);
        m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();

        D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
        ID3D12DescriptorHeap *rtv_heap;
        heap_desc.NumDescriptors = RENDER_TARGET_COUNT;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rc = m_device->CreateDescriptorHeap(&heap_desc,
                                            IID_PPV_ARGS(&rtv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_rtv_heap.reset(rtv_heap);

        m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());

            for (uint32_t n = 0; n < RENDER_TARGET_COUNT; n++) {
                ID3D12Resource *rt;
                rc = m_swap_chain->GetBuffer(n, IID_PPV_ARGS(&rt));
                CHECK_HRESULT(rc, IDXGISwapChain, GetBuffer);
                m_render_targets[n].reset(rt);
                m_device->CreateRenderTargetView(m_render_targets[n].get(), nullptr, rtv_handle);
                rtv_handle.Offset(1, m_rtv_descriptor_size);
            }
        }
        ID3D12CommandAllocator *command_alloc = nullptr;
        rc = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_alloc));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);
        m_command_allocator.reset(command_alloc);


    }

    void
    render_context::init_context(const system_config& config)
    {
        create_factory();
        select_adapter();
        create_command_queue();
        create_swap_chain();
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type,
                                        mge::buffer_change_policy change_policy,
                                        mge::buffer_access cpu_access,
                                        mge::buffer_access gpu_access,
                                        size_t element_count,
                                        void *initial_data)
    {
        mge::index_buffer_ref result;
        return result;
    }

    mge::vertex_buffer_ref 
    render_context::create_vertex_buffer(const mge::vertex_layout& layout,
                                         mge::buffer_change_policy change_policy,
                                         mge::buffer_access cpu_access,
                                         mge::buffer_access gpu_access,
                                         size_t element_count,
                                         void *initial_data)
    {
        mge::vertex_buffer_ref result;
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

}

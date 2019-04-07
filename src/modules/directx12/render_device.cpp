// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_device.hpp"
#include "window.hpp"
#include "error.hpp"
#include "moge/core/log.hpp"
#include "moge/core/utf8.hpp"
#include "moge/core/system_error.hpp"



namespace dx12 {
    render_device::render_device(window *w, const system_config& config)
        :m_window(w),
         m_feature_level((D3D_FEATURE_LEVEL) 0),
         m_frame_index(0),
         m_rtv_descriptor_size(0)
    {
        init();
    }

    render_device::~render_device()
    {
    }

    moge::swap_chain_ref
    render_device::swap_chain() const
    {
        moge::swap_chain_ref result;
        return result;
    }

    void
    render_device::setup_device(IDXGIAdapter3 *adapter)
    {
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_12_1,
                                              D3D_FEATURE_LEVEL_12_0,
                                              D3D_FEATURE_LEVEL_11_1,
                                              D3D_FEATURE_LEVEL_11_0, };
        ID3D12Device *device;
        HRESULT hr = 0;
        for(const auto level : featurelevels) {
            hr = D3D12CreateDevice(adapter,
                                   level,
                                   IID_ID3D12Device,
                                   (void **)&device);
            if (SUCCEEDED(hr)) {
                m_feature_level = level;
                m_device.reset(device);
                break;
            }
        }
    }

    void
    render_device::init()
    {
        MOGE_DEBUG_LOG(DX12) << "Create DXGI factory" << std::endl;
        IDXGIFactory4 *factory = nullptr;
        HRESULT rc = CreateDXGIFactory1(__uuidof(IDXGIFactory4), (void **) &factory);
        CHECK_HRESULT(rc, , CreateDXGIFactory1);
        if (!factory) {
            throw MOGE_EXCEPTION(moge::null_pointer);
        }
        m_dxgi_factory.reset(factory);
        MOGE_DEBUG_LOG(DX12) << "DXGI factory created" << std::endl;

        MOGE_DEBUG_LOG(DX12) << "Select adapter" << std::endl;
        IDXGIAdapter3 *adapter = nullptr;
        uint32_t index = 0;
        while(m_dxgi_factory->EnumAdapters(index, (IDXGIAdapter **)&adapter) != DXGI_ERROR_NOT_FOUND) {
            MOGE_DEBUG_LOG(DX12) << "Adapter " << index << ":" << std::endl;
            DXGI_ADAPTER_DESC1 desc;
            rc = adapter->GetDesc1(&desc);
            if(SUCCEEDED(rc)) {
                std::string desc_str;
                moge::to_utf8(desc_str, &desc.Description[0]);
                MOGE_DEBUG_LOG(DX12) << "  " << desc_str << std::endl
                                     << "  Vendor: " << std::hex << desc.VendorId << std::dec << std::endl;
            }
            if((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0) {
                setup_device(adapter);
                if(m_device) {
                    m_adapter.reset(adapter);
                    break;
                }
            } else {
                MOGE_DEBUG_LOG(DX12) << "Skipping software driver" << std::endl;
                adapter->Release();
            }
            ++index;
        }
        MOGE_DEBUG_LOG(DX12) << "Adapter selected" << std::endl;

        MOGE_DEBUG_LOG(DX12) << "Create global command queue" << std::endl;
        ID3D12CommandQueue *command_queue = nullptr;
        D3D12_COMMAND_QUEUE_DESC qdesc = {};
        qdesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        qdesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        rc = m_device->CreateCommandQueue(&qdesc, IID_PPV_ARGS(&command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);
        m_command_queue.reset(command_queue);

        MOGE_DEBUG_LOG(DX12) << "Create swap chain" << std::endl;

        DXGI_SWAP_CHAIN_DESC1 swdesc = {};
        swdesc.BufferCount = FRAME_COUNT;
        swdesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swdesc.SampleDesc.Count = 1; // TODO: multisampling
        swdesc.SwapEffect = (DXGI_SWAP_EFFECT)4; // DXGI_SWAP_EFFECT_FLIP_DISCARD;

        IDXGISwapChain3 *swap_chain = nullptr;
        rc = m_dxgi_factory->CreateSwapChainForHwnd(m_command_queue.get(),
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
        heap_desc.NumDescriptors = FRAME_COUNT;
        heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rc = m_device->CreateDescriptorHeap(&heap_desc,
                                            IID_PPV_ARGS(&rtv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_rtv_heap.reset(rtv_heap);

        m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());

            for (uint32_t n = 0; n < FRAME_COUNT; n++) {
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
}

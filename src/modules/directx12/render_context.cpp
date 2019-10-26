#include "render_context.hpp"
#include "window.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/system_error.hpp"
#include "win32/com_ptr.hpp"

namespace dx12 {

    render_context::render_context(window *win,
                                   const system_config& config)
        : mge::render_context(win)
        ,m_fence_event(0)
        ,m_fence_value(1)
        ,m_frame_index(0)
        ,m_rtv_descriptor_size(0)
    {
        HRESULT rc = 0;
        UINT factory_flags = 0;
        if (config.debug()) {
            factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        mge::com_ptr<IDXGIFactory4> factory;
        rc = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&factory));
        CHECK_HRESULT(rc, ,CreateDXGIFactory2);

        mge::com_ptr<IDXGIAdapter1> adapter;
        if (config.warp()) {
            rc = factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
            CHECK_HRESULT(rc, IDXGIFactory4, EnumWarpAdapter);
        } else {
            select_adapter(factory.Get(), &adapter);

        }
        rc = D3D12CreateDevice(adapter.Get(),
                               D3D_FEATURE_LEVEL_11_0,
                               IID_PPV_ARGS(&m_device));
        CHECK_HRESULT(rc, ,D3D12CreateDevice);

        create_command_queue();
        create_swap_chain(factory.Get(), win);
        create_descriptor_heap();
        create_frame_resources();
        create_command_allocator();
        create_graphics_command_list();
        create_sync_objects();
    }

    void
    render_context::create_sync_objects()
    {
        auto rc = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
        CHECK_HRESULT(rc, ID3D12Device, CreateFence);
        m_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!m_fence_event) {

        }
    }

    void
    render_context::create_graphics_command_list()
    {
        auto rc = m_device->CreateCommandList
                (0,
                 D3D12_COMMAND_LIST_TYPE_DIRECT,
                 m_command_allocator.Get(),
                 nullptr,
                 IID_PPV_ARGS(&m_command_list));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandList);
        rc = m_command_list->Close();
        CHECK_HRESULT(rc, ID3D12CommandList, Close);
    }

    void
    render_context::create_command_allocator()
    {
        auto rc = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);
    }

    void
    render_context::create_frame_resources()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle
                (m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
        m_render_targets.resize(frame_count());

        for (uint32_t i=0; i<m_render_targets.size(); ++i) {
            auto rc = m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_render_targets[i]));
            CHECK_HRESULT(rc, IDXGISwapChain3, GetBuffer);
            m_device->CreateRenderTargetView(m_render_targets[i].Get(), nullptr, rtv_handle);
            rtv_handle.ptr += m_rtv_descriptor_size;
        }
    }

    void
    render_context::create_command_queue()
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        auto rc = m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);
    }

    void
    render_context::create_swap_chain(IDXGIFactory4 *factory,
                                      window *win)
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.BufferCount = frame_count();
        desc.Width = win->extent().width();
        desc.Height = win->extent().height();
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.SampleDesc.Count = 1;

        mge::com_ptr<IDXGISwapChain1> sc;
        auto rc = factory->CreateSwapChainForHwnd(m_command_queue.Get(),
                                                  win->hwnd(),
                                                  &desc,
                                                  nullptr,
                                                  nullptr,
                                                  &sc);
        CHECK_HRESULT(rc, IDXGIFactory, CreateSwapChainForHwnd);
        rc = factory->MakeWindowAssociation(win->hwnd(), DXGI_MWA_NO_ALT_ENTER);
        CHECK_HRESULT(rc, IDXGIFactory, MakeWindowAssociation);
        rc = sc.As(&m_swap_chain);
        CHECK_HRESULT(rc, , IDXGISwapChain4);

        m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();

    }

    void
    render_context::create_descriptor_heap()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};

        rtv_heap_desc.NumDescriptors = frame_count();
        rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        auto rc = m_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }

    void
    render_context::select_adapter(IDXGIFactory4 *factory,
                                   IDXGIAdapter1 **adapter)
    {
        mge::com_ptr<IDXGIAdapter1> local_adapter;
        *adapter = nullptr;

        for (uint32_t i=0;
             DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &local_adapter);
             i) {
            DXGI_ADAPTER_DESC1 desc = {};
            local_adapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                continue;
            } else {
                auto rc = D3D12CreateDevice(local_adapter.Get(),
                                            D3D_FEATURE_LEVEL_11_0,
                                            _uuidof(ID3D12Device),
                                            nullptr);
                if(SUCCEEDED(rc)) {
                    break;
                }
            }
        }

        *adapter = local_adapter.Detach();
        if (!adapter) {
            MGE_THROW(dx12::error) << "Could not find suitable adapter";
        }
    }

    render_context::~render_context()
    {
        if (m_fence_event) {
            CloseHandle(m_fence_event);
            m_fence_event = 0;
        }
    }

    void
    render_context::wait_for_frame()
    {
        uint64_t fence = m_fence_value;
        auto rc = m_command_queue->Signal(m_fence.Get(), fence);
        ++m_fence_value;
        CHECK_HRESULT(rc, ID3D12CommandQueue, Signal);
        if (m_fence->GetCompletedValue() < fence) {
            rc = m_fence->SetEventOnCompletion(fence, m_fence_event);
            CHECK_HRESULT(rc, ID3D12Fence, SetEventOnCompletion);
            WaitForSingleObject(m_fence_event, INFINITE);
        }
        m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    }

    void
    render_context::flush()
    {

        wait_for_frame();

    }

    mge::vertex_buffer_ref
    render_context::create_vertex_buffer(const mge::vertex_layout& layout,
                                         mge::usage usage,
                                         size_t element_count,
                                         void *initial_data)
    {
       MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type,
                                        mge::usage usage,
                                        size_t element_count,
                                        void *initial_data)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d(const mge::image_ref& image)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::shader_ref
    render_context::create_shader(mge::shader_type type)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::command_list_ref
    render_context::create_command_list()
    {
        return std::make_shared<mge::memory_command_list>(*this);
    }

    void
    render_context::execute(const mge::command_list_ref& commands)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    mge::pipeline_ref
    render_context::create_pipeline()
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void
    render_context::shader_languages(std::vector<mge::shader_language>& languages) const
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }
}

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/win32/com_ptr.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "vertex_buffer.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    static inline D3D12_BLEND blend_factor_to_dx12(blend_factor factor)
    {
        switch (factor) {
        case blend_factor::ZERO:
            return D3D12_BLEND_ZERO;
        case blend_factor::ONE:
            return D3D12_BLEND_ONE;
        case blend_factor::SRC_COLOR:
            return D3D12_BLEND_SRC_COLOR;
        case blend_factor::ONE_MINUS_SRC_COLOR:
            return D3D12_BLEND_INV_SRC_COLOR;
        case blend_factor::DST_COLOR:
            return D3D12_BLEND_DEST_COLOR;
        case blend_factor::ONE_MINUS_DST_COLOR:
            return D3D12_BLEND_INV_DEST_COLOR;
        case blend_factor::SRC_ALPHA:
            return D3D12_BLEND_SRC_ALPHA;
        case blend_factor::ONE_MINUS_SRC_ALPHA:
            return D3D12_BLEND_INV_SRC_ALPHA;
        case blend_factor::DST_ALPHA:
            return D3D12_BLEND_DEST_ALPHA;
        case blend_factor::ONE_MINUS_DST_ALPHA:
            return D3D12_BLEND_INV_DEST_ALPHA;
        case blend_factor::CONSTANT_COLOR:
            return D3D12_BLEND_BLEND_FACTOR;
        case blend_factor::ONE_MINUS_CONSTANT_COLOR:
            return D3D12_BLEND_INV_BLEND_FACTOR;
        case blend_factor::CONSTANT_ALPHA:
            return D3D12_BLEND_BLEND_FACTOR;
        case blend_factor::ONE_MINUS_CONSTANT_ALPHA:
            return D3D12_BLEND_INV_BLEND_FACTOR;
        case blend_factor::SRC_ALPHA_SATURATE:
            return D3D12_BLEND_SRC_ALPHA_SAT;
        case blend_factor::SRC1_COLOR:
            return D3D12_BLEND_SRC1_COLOR;
        case blend_factor::ONE_MINUS_SRC1_COLOR:
            return D3D12_BLEND_INV_SRC1_COLOR;
        case blend_factor::SRC1_ALPHA:
            return D3D12_BLEND_SRC1_ALPHA;
        case blend_factor::ONE_MINUS_SRC1_ALPHA:
            return D3D12_BLEND_INV_SRC1_ALPHA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend factor: " << factor;
        }
    }

    static inline D3D12_BLEND_OP blend_operation_to_dx12(blend_operation op)
    {
        switch (op) {
        case blend_operation::NONE:
            return D3D12_BLEND_OP_ADD;
        case blend_operation::ADD:
            return D3D12_BLEND_OP_ADD;
        case blend_operation::SUBTRACT:
            return D3D12_BLEND_OP_SUBTRACT;
        case blend_operation::REVERSE_SUBTRACT:
            return D3D12_BLEND_OP_REV_SUBTRACT;
        case blend_operation::MIN:
            return D3D12_BLEND_OP_MIN;
        case blend_operation::MAX:
            return D3D12_BLEND_OP_MAX;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend operation: " << op;
        }
    }

    static inline D3D12_COMPARISON_FUNC depth_test_to_dx12(mge::test func)
    {
        switch (func) {
        case mge::test::NEVER:
            return D3D12_COMPARISON_FUNC_NEVER;
        case mge::test::LESS:
            return D3D12_COMPARISON_FUNC_LESS;
        case mge::test::EQUAL:
            return D3D12_COMPARISON_FUNC_EQUAL;
        case mge::test::LESS_EQUAL:
            return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case mge::test::GREATER:
            return D3D12_COMPARISON_FUNC_GREATER;
        case mge::test::NOT_EQUAL:
            return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        case mge::test::GREATER_EQUAL:
            return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        case mge::test::ALWAYS:
            return D3D12_COMPARISON_FUNC_ALWAYS;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown depth test: " << static_cast<int>(func);
        }
    }

    render_context::render_context(mge::dx12::render_system& render_system_,
                                   mge::dx12::window&        window_)
        : mge::render_context(render_system_, window_.extent())
        , m_render_system(render_system_)
        , m_window(window_)
        , m_command_queue_fence_value(0)
        , m_command_queue_fence_event(0)
        , m_rtv_descriptor_size(0)
        , m_dsv_descriptor_size(0)
        , m_callback_cookie(0)
        , m_data_lock("render_context")
    {
        m_viewport = {0.0f,
                      0.0f,
                      static_cast<float>(window_.extent().width),
                      static_cast<float>(window_.extent().height),
                      0.0f,
                      1.0f};
        m_scissor_rect = {0,
                          0,
                          static_cast<LONG>(window_.extent().width),
                          static_cast<LONG>(window_.extent().height)};
        enable_debug_layer();
        create_factory();
        create_adapter();
        create_device();

        auto fd = m_render_system.frame_debugger();
        if (fd) {
            fd->set_context(frame_debugger::capture_context{m_device.Get(),
                                                            m_window.hwnd()});
        }
        enable_debug_messages();
        create_command_queue();

        m_rasterizer_desc = {
            .FillMode = D3D12_FILL_MODE_SOLID,
            .CullMode = D3D12_CULL_MODE_BACK,
            .FrontCounterClockwise = FALSE,
            .DepthBias = D3D12_DEFAULT_DEPTH_BIAS,
            .DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP,
            .SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
            .DepthClipEnable = TRUE,
            .MultisampleEnable = FALSE,
            .AntialiasedLineEnable = FALSE,
            .ForcedSampleCount = 0,
            .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF};

        m_blend_desc_no_blend = {.AlphaToCoverageEnable = FALSE,
                                 .IndependentBlendEnable = FALSE};

        m_blend_desc_no_blend.RenderTarget[0] = {
            .BlendEnable = FALSE,
            .LogicOpEnable = FALSE,
            .SrcBlend = D3D12_BLEND_ONE,
            .DestBlend = D3D12_BLEND_ZERO,
            .BlendOp = D3D12_BLEND_OP_ADD,
            .SrcBlendAlpha = D3D12_BLEND_ONE,
            .DestBlendAlpha = D3D12_BLEND_ZERO,
            .BlendOpAlpha = D3D12_BLEND_OP_ADD,
            .LogicOp = D3D12_LOGIC_OP_NOOP,
            .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL};
    }

    void render_context::create_command_queue()
    {
        MGE_DEBUG_TRACE(DX12, "Create command queue");
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;
        auto rc =
            m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);

        // m_command_queue->SetName(L"mge::dx12::render_context::m_command_queue");
        MGE_DEBUG_TRACE(DX12, "Create command queue fence");
        rc = m_device->CreateFence(0,
                                   D3D12_FENCE_FLAG_NONE,
                                   IID_PPV_ARGS(&m_command_queue_fence));
        CHECK_HRESULT(rc, ID3D12Device, CreateFence);
        m_command_queue_fence_event =
            CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (m_command_queue_fence_event == 0) {
            MGE_THROW(dx12::error) << "CreateEvent failed";
        }
        // m_command_queue->SetName(
        //    L"mge::dx12::render_context::m_command_queue_fence");
    }

    void render_context::create_device()
    {
        MGE_DEBUG_TRACE(DX12, "Create D3D12Device");
        auto rc = D3D12CreateDevice(m_adapter.Get(),
                                    D3D_FEATURE_LEVEL_11_0,
                                    IID_PPV_ARGS(&m_device));
        CHECK_HRESULT(rc, , D3D12CreateDevice);
    }

    void render_context::create_factory()
    {
        UINT factory_flags = 0;
        if (m_render_system.debug()) {
            factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        MGE_DEBUG_TRACE(DX12, "Create DXGI Factory");
        auto rc = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory));
        CHECK_HRESULT(rc, , CreateDXGIFactory2);
    }

    void render_context::create_adapter()
    {
        MGE_DEBUG_TRACE(DX12, "Create DXGI Adapter");
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
    void render_context::create_swap_chain()
    {
        MGE_DEBUG_TRACE(DX12, "Create swap chain");
        DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
        swap_chain_desc.Width = m_window.extent().width;
        swap_chain_desc.Height = m_window.extent().height;
        swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.Stereo = FALSE;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.SampleDesc.Quality = 0;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount = buffer_count;
        swap_chain_desc.Scaling = DXGI_SCALING_STRETCH;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swap_chain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swap_chain_desc.Flags = 0;
        // DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
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
        m_rtv_heap->SetName(L"mge::dx12::render_context::m_rtv_heap");

        MGE_DEBUG_TRACE(DX12, "Create descriptor heap for render target views");

        D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
        dsv_heap_desc.NumDescriptors = buffer_count;
        dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rc = m_device->CreateDescriptorHeap(&dsv_heap_desc,
                                            IID_PPV_ARGS(&m_dsv_heap));
        CHECK_HRESULT(rc, ID3D12Device, CreateDescriptorHeap);
        m_dsv_heap->SetName(L"mge::dx12::render_context::m_dsv_heap");
        m_dsv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
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
            std::wstringstream ws;
            ws << "mge::dx12::render_context::backbuffer#" << i;
            backbuffer->SetName(ws.str().c_str());
            CHECK_HRESULT(rc, IDXGISwapChain4, GetBuffer);
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
            D3D12_RESOURCE_DESC depth_desc = {};
            depth_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
            depth_desc.Alignment = 0;
            depth_desc.Width = m_window.extent().width;
            depth_desc.Height = m_window.extent().height;
            depth_desc.DepthOrArraySize = 1;
            depth_desc.MipLevels = 1;
            depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depth_desc.SampleDesc.Count = 1;
            depth_desc.SampleDesc.Quality = 0;
            depth_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
            depth_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

            D3D12_CLEAR_VALUE clear_value = {};
            clear_value.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            clear_value.DepthStencil.Depth = 1.0f;
            clear_value.DepthStencil.Stencil = 0;

            D3D12_HEAP_PROPERTIES heap_props = {};
            heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;
            heap_props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heap_props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            heap_props.CreationNodeMask = 1;
            heap_props.VisibleNodeMask = 1;

            mge::com_ptr<ID3D12Resource> depth_buffer;
            auto                         rc = m_device->CreateCommittedResource(
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

            D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
            dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
            dsv_desc.Flags = D3D12_DSV_FLAG_NONE;

            m_device->CreateDepthStencilView(depth_buffer.Get(),
                                             &dsv_desc,
                                             dsv_handle);
            m_depth_buffers.emplace_back(depth_buffer);
            dsv_handle.ptr += m_dsv_descriptor_size;
        }
    }

    mge::com_ptr<ID3D12CommandAllocator>
    render_context::create_command_allocator(D3D12_COMMAND_LIST_TYPE type,
                                             const char*             purpose)
    {
        if (purpose) {
            MGE_DEBUG_TRACE(DX12, "Create command allocator for {}", purpose);
        }
        mge::com_ptr<ID3D12CommandAllocator> result;
        auto rc = m_device->CreateCommandAllocator(type, IID_PPV_ARGS(&result));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);
        // if (purpose) {
        //     std::wstringstream ws;
        //     ws << "mge::dx12::render_context::command_allocator#" << purpose;
        //     result->SetName(ws.str().c_str());
        // }
        return result;
    }

    mge::com_ptr<ID3D12GraphicsCommandList>
    render_context::create_dx12_command_list(ID3D12CommandAllocator* allocator,
                                             D3D12_COMMAND_LIST_TYPE type,
                                             const char*             purpose,
                                             bool                    reset)
    {
        if (purpose) {
            MGE_DEBUG_TRACE(DX12, "Create command list for {}", purpose);
        }
        mge::com_ptr<ID3D12GraphicsCommandList> result;
        auto rc = m_device->CreateCommandList(0,
                                              type,
                                              allocator,
                                              nullptr,
                                              IID_PPV_ARGS(&result));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandList);
        // if (purpose) {
        //     std::wstringstream ws;
        //     ws << "mge::dx12::render_context::command_list#" << purpose;
        //     result->SetName(ws.str().c_str());
        // }

        if (!reset) {
            return result;
        }

        result->Close();
        rc = allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc = result->Reset(allocator, nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
        return result;
    }

    void render_context::create_command_lists()
    {
        m_xfer_command_allocator =
            create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "resource transfer");
        m_command_allocator =
            create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "main command allocator");

        m_xfer_command_list =
            create_dx12_command_list(m_xfer_command_allocator.Get(),
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "resource transfer");
        m_command_list =
            create_dx12_command_list(m_command_allocator.Get(),
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "main command list",
                                     false);
        m_command_list->Close();
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

    render_context::~render_context()
    {
        if (m_render_system.frame_debugger()) {
            auto fd = m_render_system.frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(DX12, "Ending frame recording");
                fd->end_capture();
            }
        }

        if (m_info_queue && m_callback_cookie != 0) {
            m_info_queue->UnregisterMessageCallback(m_callback_cookie);
        }
    }

    mge::index_buffer* render_context::on_create_index_buffer(mge::data_type dt,
                                                              size_t data_size)
    {
        return new dx12::index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer*
    render_context::on_create_vertex_buffer(const mge::vertex_layout& layout,
                                            size_t                    data_size)
    {
        return new dx12::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context::on_create_shader(shader_type t)
    {
        auto result = new dx12::shader(*this, t);
        return result;
    }

    mge::program* render_context::on_create_program()
    {
        return new dx12::program(*this);
    }

    void render_context::copy_resource_sync(ID3D12Resource*       dst,
                                            ID3D12Resource*       src,
                                            D3D12_RESOURCE_STATES state_after)
    {
        // MGE_DEBUG_TRACE(DX12)
        //    << "Copy resource " << (void*)src << " to " << (void*)dst;

        std::lock_guard<mge::mutex> lock(m_data_lock);

        m_xfer_command_list->CopyResource(dst, src);
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = dst;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
        barrier.Transition.StateAfter = state_after;
        barrier.Transition.Subresource =
            D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        m_xfer_command_list->ResourceBarrier(1, &barrier);
        m_xfer_command_list->Close();
        ID3D12CommandList* lists[] = {m_xfer_command_list.Get()};
        m_command_queue->ExecuteCommandLists(1, lists);
        wait_for_command_queue();

        auto rc = m_xfer_command_allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc =
            m_xfer_command_list->Reset(m_xfer_command_allocator.Get(), nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);

        // MGE_DEBUG_TRACE(DX12)
        //    << "Resource copy done: " << (void*)src << " to " << (void*)dst;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE render_context::rtv_handle(uint32_t index) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE result =
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart();
        result.ptr += m_rtv_descriptor_size * index;
        return result;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE render_context::dsv_handle(uint32_t index) const
    {
        D3D12_CPU_DESCRIPTOR_HANDLE result =
            m_dsv_heap->GetCPUDescriptorHandleForHeapStart();
        result.ptr += m_dsv_descriptor_size * index;
        return result;
    }

    void render_context::wait_for_command_queue()
    {
        // wait for frame
        auto fence = ++m_command_queue_fence_value;
        // MGE_DEBUG_TRACE(DX12, "Waiting for command queue fence: {}", fence);
        auto rc = m_command_queue->Signal(m_command_queue_fence.Get(), fence);
        CHECK_HRESULT(rc, ID3D12CommandQueue, Signal);
        uint64_t fence_completed_value =
            m_command_queue_fence->GetCompletedValue();
        if (fence_completed_value < fence) {
            rc = m_command_queue_fence->SetEventOnCompletion(
                fence,
                m_command_queue_fence_event);
            CHECK_HRESULT(rc, ID3D12CommandQueue, SetEventOnCompletion);
            DWORD wait_rc =
                WaitForSingleObject(m_command_queue_fence_event, 10000);
            if (wait_rc == WAIT_FAILED) {
                MGE_CHECK_SYSTEM_ERROR(WaitForSingleObject);
            } else if (wait_rc == WAIT_TIMEOUT) {
                MGE_THROW(dx12::error)
                    << "Wait for command queue fence timed out";
            }
            if (!ResetEvent(m_command_queue_fence_event)) {
                MGE_CHECK_SYSTEM_ERROR(ResetEvent);
            }
        }
        // MGE_DEBUG_TRACE(DX12, "Frame completed:
        // {}",m_command_queue_fence_value);
    }

    mge::texture_ref render_context::create_texture(texture_type type)
    {
        mge::texture_ref result;
        return result;
    }

    void render_context::enable_debug_layer()
    {
        if (m_render_system.debug()) {
            MGE_DEBUG_TRACE(DX12, "Enable debug layer");
            auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug));
            CHECK_HRESULT(hr, , D3D12GetDebugInterface);
            m_debug->EnableDebugLayer();
        }
    }

    void render_context::enable_debug_messages()
    {
        if (m_render_system.debug()) {
            MGE_DEBUG_TRACE(DX12, "Enabling debug messages");
            if (SUCCEEDED(m_device.As(&m_info_queue))) {
                HRESULT rc = S_OK;
#if 0
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
                deny_filter.DenyList.NumSeverities = 0;
                deny_filter.DenyList.pSeverityList = nullptr;
                deny_filter.DenyList.NumIDs =
                    checked_cast<UINT>(array_size(denied_message_ids));
                deny_filter.DenyList.pIDList = denied_message_ids;
                rc = m_info_queue->PushStorageFilter(&deny_filter);
                CHECK_HRESULT(rc, ID3D12InfoQueue, PushStorageFilter);
#endif
                rc = m_info_queue->RegisterMessageCallback(
                    &message_func,
                    D3D12_MESSAGE_CALLBACK_IGNORE_FILTERS,
                    nullptr,
                    &m_callback_cookie);
                CHECK_HRESULT(rc, ID3D12InfoQueue1, RegisterMessageCallback);
            }
        }
    }

    static const char* message_category(D3D12_MESSAGE_CATEGORY category)
    {
        switch (category) {
        case D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED:
            return "APPLICATION_DEFINED";
        case D3D12_MESSAGE_CATEGORY_MISCELLANEOUS:
            return "MISCELLANEOUS";
        case D3D12_MESSAGE_CATEGORY_INITIALIZATION:
            return "INITIALIZATION";
        case D3D12_MESSAGE_CATEGORY_CLEANUP:
            return "CLEANUP";
        case D3D12_MESSAGE_CATEGORY_COMPILATION:
            return "COMPILATION";
        case D3D12_MESSAGE_CATEGORY_STATE_CREATION:
            return "STATE_CREATION";
        case D3D12_MESSAGE_CATEGORY_STATE_SETTING:
            return "STATE_SETTING";
        case D3D12_MESSAGE_CATEGORY_STATE_GETTING:
            return "STATE_GETTING";
        case D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION:
            return "RESOURCE_MANIPULATION";
        case D3D12_MESSAGE_CATEGORY_EXECUTION:
            return "EXECUTION";
        case D3D12_MESSAGE_CATEGORY_SHADER:
            return "SHADER";
        default:
            return "UNKNOWN";
        }
    }

    static const char* message_severity(D3D12_MESSAGE_SEVERITY severity)
    {
        switch (severity) {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            return "CORRUPTION";
        case D3D12_MESSAGE_SEVERITY_ERROR:
            return "ERROR";
        case D3D12_MESSAGE_SEVERITY_WARNING:
            return "WARNING";
        case D3D12_MESSAGE_SEVERITY_INFO:
            return "INFO";
        default:
            return "UNKNOWN";
        }
    }

    void render_context::message_func(D3D12_MESSAGE_CATEGORY category,
                                      D3D12_MESSAGE_SEVERITY severity,
                                      D3D12_MESSAGE_ID       id,
                                      LPCSTR                 description,
                                      void*                  context)
    {
        switch (severity) {
        case D3D12_MESSAGE_SEVERITY_CORRUPTION:
            MGE_ERROR_TRACE(DX12,
                            "DirectX12 Debug [{}] ({}) ID: {} - {}",
                            message_severity(severity),
                            message_category(category),
                            id,
                            description);
            break;
        case D3D12_MESSAGE_SEVERITY_ERROR:
            MGE_ERROR_TRACE(DX12,
                            "DirectX12 Debug [{}] ({}) ID: {} - {}",
                            message_severity(severity),
                            message_category(category),
                            id,
                            description);
            break;
        case D3D12_MESSAGE_SEVERITY_WARNING:
            MGE_WARNING_TRACE(DX12,
                              "DirectX12 Debug [{}] ({}) ID: {} - {}",
                              message_severity(severity),
                              message_category(category),
                              id,
                              description);
            break;
        case D3D12_MESSAGE_SEVERITY_INFO:
        default:
            MGE_INFO_TRACE(DX12,
                           "DirectX12 Debug [{}] ({}) ID: {} - {}",
                           message_severity(severity),
                           message_category(category),
                           id,
                           description);
            break;
        }
    }

    void render_context::render(const mge::pass& p)
    {
        ID3D12GraphicsCommandList* pass_command_list = nullptr;
        uint32_t                   current_buffer_index = 0;

        if (!p.frame_buffer()) {
            pass_command_list = m_command_list.Get();
            current_buffer_index = m_swap_chain->GetCurrentBackBufferIndex();
            if (m_draw_state != draw_state::DRAW) {
                // MGE_DEBUG_TRACE(DX12, "Waiting for frame to be finished");
                wait_for_command_queue();
                // MGE_DEBUG_TRACE(DX12, "Reset command list for new frame");
                auto rc = m_command_allocator->Reset();
                CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
                rc = m_command_list->Reset(m_command_allocator.Get(), nullptr);
                CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
                // MGE_DEBUG_TRACE(DX12, "Setup resource barrier present to
                // render");
                D3D12_RESOURCE_BARRIER present_to_render = {
                    .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
                    .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
                    .Transition =
                        {.pResource = m_backbuffers[current_buffer_index].Get(),
                         .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                         .StateBefore = D3D12_RESOURCE_STATE_PRESENT,
                         .StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET},
                };
                pass_command_list->ResourceBarrier(1, &present_to_render);
                D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle =
                    m_rtv_heap->GetCPUDescriptorHandleForHeapStart();
                rtv_handle.ptr += m_rtv_descriptor_size * current_buffer_index;

                D3D12_CPU_DESCRIPTOR_HANDLE dsv =
                    dsv_handle(current_buffer_index);
                pass_command_list->OMSetRenderTargets(1,
                                                      &rtv_handle,
                                                      FALSE,
                                                      &dsv);
                m_draw_state = draw_state::DRAW;
            }
        } else {
            // frame buffer specific command list
        }

        const auto&    vp = p.viewport();
        D3D12_VIEWPORT d3d12_viewport =
            {vp.x, vp.y, vp.width, vp.height, vp.min_depth, vp.max_depth};
        pass_command_list->RSSetViewports(1, &d3d12_viewport);

        const auto& sc = p.scissor();
        D3D12_RECT  d3d12_scissor = {static_cast<LONG>(sc.left),
                                     static_cast<LONG>(sc.top),
                                     static_cast<LONG>(sc.right),
                                     static_cast<LONG>(sc.bottom)};
        pass_command_list->RSSetScissorRects(1, &d3d12_scissor);

        if (p.clear_color_enabled()) {
            const auto& color = p.clear_color_value();
            FLOAT       clear_color[4] = {color.r, color.g, color.b, color.a};
            pass_command_list->ClearRenderTargetView(
                rtv_handle(current_buffer_index),
                clear_color,
                0,
                nullptr);
        }

        if (p.clear_depth_enabled()) {
            FLOAT depth_value = static_cast<FLOAT>(p.clear_depth_value());
            pass_command_list->ClearDepthStencilView(
                dsv_handle(current_buffer_index),
                D3D12_CLEAR_FLAG_DEPTH,
                depth_value,
                0,
                0,
                nullptr);
        }
        bool blend_pass_needed = false;
        p.for_each_draw_command([&](const mge::program_handle&       program,
                                    const mge::vertex_buffer_handle& vertices,
                                    const mge::index_buffer_handle&  indices,
                                    const mge::pipeline_state&       state,
                                    mge::uniform_block* /*ub*/) {
            auto blend_operation = state.color_blend_operation();
            if (blend_operation == blend_operation::NONE) {
                draw_geometry(pass_command_list,
                              program.get(),
                              vertices.get(),
                              indices.get(),
                              state);
            } else {
                blend_pass_needed = true;
            }
        });

        if (blend_pass_needed) {
            p.for_each_draw_command(
                [&](const mge::program_handle&       program,
                    const mge::vertex_buffer_handle& vertices,
                    const mge::index_buffer_handle&  indices,
                    const mge::pipeline_state&       state,
                    mge::uniform_block* /*ub*/) {
                    draw_geometry(pass_command_list,
                                  program.get(),
                                  vertices.get(),
                                  indices.get(),
                                  state);
                });
        }
    }

    void render_context::draw_geometry(ID3D12GraphicsCommandList* command_list,
                                       mge::program*              program,
                                       mge::vertex_buffer*        vb,
                                       mge::index_buffer*         ib,
                                       const mge::pipeline_state& state)
    {
        auto dx12_program = static_cast<dx12::program*>(program);
        if (!dx12_program) {
            MGE_THROW(mge::illegal_state)
                << "Draw command has no program assigned";
        }
        const auto& pipeline_state = static_pipeline_state(dx12_program, state);
        if (!pipeline_state.Get()) {
            MGE_THROW(mge::illegal_state)
                << "Failed to get pipeline state for program";
        }
        auto root_signature = dx12_program->root_signature();
        command_list->SetGraphicsRootSignature(root_signature);
        command_list->SetPipelineState(pipeline_state.Get());
        command_list->IASetPrimitiveTopology(
            D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        auto dx12_vertices = static_cast<dx12::vertex_buffer*>(vb);
        command_list->IASetVertexBuffers(0, 1, &(dx12_vertices->view()));
        auto dx12_indices = static_cast<dx12::index_buffer*>(ib);
        command_list->IASetIndexBuffer(&(dx12_indices->view()));
        command_list->DrawIndexedInstanced(
            static_cast<UINT>(dx12_indices->element_count()),
            1,
            0,
            0,
            0);
    }

    mge::image_ref render_context::screenshot()
    {
        return mge::image_ref();
    }

    void render_context::on_frame_present()
    {
        D3D12_RESOURCE_BARRIER render_to_present = {
            .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
            .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
            .Transition = {
                .pResource =
                    m_backbuffers[m_swap_chain->GetCurrentBackBufferIndex()]
                        .Get(),
                .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                .StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
                .StateAfter = D3D12_RESOURCE_STATE_PRESENT}};
        m_command_list->ResourceBarrier(1, &render_to_present);

        m_command_list->Close();
        ID3D12CommandList* lists[] = {m_command_list.Get()};
        m_command_queue->ExecuteCommandLists(1, lists);
        m_draw_state = draw_state::SUBMIT;
        m_swap_chain->Present(0, 0);
        m_draw_state = draw_state::NONE;
    }

    const mge::com_ptr<ID3D12PipelineState>&
    render_context::static_pipeline_state(mge::dx12::program*        program,
                                          const mge::pipeline_state& state)
    {
        pipeline_state_key key = std::make_tuple(program, state);

        {
            std::lock_guard<mge::mutex> lock(m_data_lock);
            auto it = m_program_pipeline_states.find(key);
            if (it != m_program_pipeline_states.end()) {
                return it->second;
            }
        }
        auto vs_ptr = program->program_shader(shader_type::VERTEX);
        auto ps_ptr = program->program_shader(shader_type::FRAGMENT);

        auto& vs = dx12_shader(*vs_ptr);
        auto& ps = dx12_shader(*ps_ptr);
        auto  root_signature = program->root_signature();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = {};
        pso_desc.InputLayout = {vs.input_layout(), vs.input_layout_count()};
        pso_desc.pRootSignature = root_signature;
        pso_desc.VS = {vs.code()->GetBufferPointer(),
                       vs.code()->GetBufferSize()};
        pso_desc.PS = {ps.code()->GetBufferPointer(),
                       ps.code()->GetBufferSize()};

        pso_desc.RasterizerState = m_rasterizer_desc;
        if (blend_operation::NONE == state.color_blend_operation()) {
            pso_desc.BlendState = m_blend_desc_no_blend;
        } else {
            blend_operation color_op = state.color_blend_operation();
            blend_factor    color_src = state.color_blend_factor_src();
            blend_factor    color_dst = state.color_blend_factor_dst();
            blend_operation alpha_op = state.alpha_blend_operation();
            blend_factor    alpha_src = state.alpha_blend_factor_src();
            blend_factor    alpha_dst = state.alpha_blend_factor_dst();

            pso_desc.BlendState = {.AlphaToCoverageEnable = FALSE,
                                   .IndependentBlendEnable = FALSE};

            pso_desc.BlendState.RenderTarget[0] = {
                .BlendEnable = TRUE,
                .LogicOpEnable = FALSE,
                .SrcBlend = blend_factor_to_dx12(color_src),
                .DestBlend = blend_factor_to_dx12(color_dst),
                .BlendOp = blend_operation_to_dx12(color_op),
                .SrcBlendAlpha = blend_factor_to_dx12(alpha_src),
                .DestBlendAlpha = blend_factor_to_dx12(alpha_dst),
                .BlendOpAlpha = blend_operation_to_dx12(alpha_op),
                .LogicOp = D3D12_LOGIC_OP_NOOP,
                .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL};
        }
        pso_desc.DepthStencilState = {
            .DepthEnable = TRUE,
            .DepthWriteMask = state.depth_write() ? D3D12_DEPTH_WRITE_MASK_ALL
                                                  : D3D12_DEPTH_WRITE_MASK_ZERO,
            .DepthFunc = depth_test_to_dx12(state.depth_test_function()),
            .StencilEnable = FALSE};
        pso_desc.SampleMask = UINT_MAX;
        pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        pso_desc.NumRenderTargets = 1;
        pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        pso_desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        pso_desc.SampleDesc = {.Count = 1, .Quality = 0};
        mge::com_ptr<ID3D12PipelineState> pipeline_state;
        auto rc = m_device->CreateGraphicsPipelineState(
            &pso_desc,
            IID_PPV_ARGS(&pipeline_state));
        CHECK_HRESULT(rc, ID3D12Device, CreateGraphicsPipelineState);
        {
            std::lock_guard<mge::mutex> lock(m_data_lock);
            m_program_pipeline_states.emplace(key, pipeline_state);
            return m_program_pipeline_states[key];
        }
    }

} // namespace mge::dx12
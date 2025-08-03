// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "frame_command_list.hpp"
#include "index_buffer.hpp"
#include "mge/core/array_size.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
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

    render_context::render_context(mge::dx12::render_system& render_system_,
                                   mge::dx12::window&        window_)
        : m_render_system(render_system_)
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
                      0.0f};
        m_scissor_rect = {0,
                          0,
                          static_cast<LONG>(window_.extent().width),
                          static_cast<LONG>(window_.extent().height)};
        enable_debug_layer();
        create_factory();
        create_adapter();
        create_device();
        enable_debug_messages();
        create_command_queue();
    }

    void render_context::create_command_queue()
    {
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create command queue";
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;
        auto rc =
            m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_command_queue));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandQueue);

        // m_command_queue->SetName(L"mge::dx12::render_context::m_command_queue");
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create command queue fence";
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
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create D3D12Device";
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
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create DXGI Factory";
        auto rc = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory));
        CHECK_HRESULT(rc, , CreateDXGIFactory2);
    }

    void render_context::create_adapter()
    {
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create DXGI Adapter";
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
        m_rtv_heap->SetName(L"mge::dx12::render_context::m_rtv_heap");

        MGE_DEBUG_TRACE_STREAM(DX12)
            << "Create descriptor heap for render target views";
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

    void render_context::update_render_target_views(
        const std::shared_ptr<mge::dx12::swap_chain>& swap_chain)
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle =
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart();

        for (int i = 0; i < buffer_count; ++i) {
            MGE_DEBUG_TRACE_STREAM(DX12)
                << "Create render target view for back buffer #" << i
                << " of swap chain";
            mge::com_ptr<ID3D12Resource> backbuffer;
            auto rc = swap_chain->dxgi_swap_chain()->GetBuffer(
                i,
                IID_PPV_ARGS(&backbuffer));
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

    mge::com_ptr<ID3D12CommandAllocator>
    render_context::create_command_allocator(D3D12_COMMAND_LIST_TYPE type,
                                             const char*             purpose)
    {
        // if (purpose) {
        //     MGE_DEBUG_TRACE(DX12) << "Create command allocator for " <<
        //     purpose;
        // }
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
                                             const char*             purpose)
    {
        // if (purpose) {
        //     MGE_DEBUG_TRACE(DX12) << "Create command list for " << purpose;
        // }
        mge::com_ptr<ID3D12GraphicsCommandList> result;
        auto rc = m_device->CreateCommandList(0,
                                              D3D12_COMMAND_LIST_TYPE_DIRECT,
                                              allocator,
                                              nullptr,
                                              IID_PPV_ARGS(&result));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandList);
        // if (purpose) {
        //     std::wstringstream ws;
        //     ws << "mge::dx12::render_context::command_list#" << purpose;
        //     result->SetName(ws.str().c_str());
        // }
        result->Close();
        rc = allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc = result->Reset(allocator, nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
        return result;
    }

    void render_context::create_command_lists()
    {
        m_begin_command_allocator =
            create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "begin of drawing");
        m_end_command_allocator =
            create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "end of drawing");
        m_xfer_command_allocator =
            create_command_allocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "resource transfer");

        m_begin_command_list =
            create_dx12_command_list(m_begin_command_allocator.Get(),
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "begin of drawing");
        m_end_command_list =
            create_dx12_command_list(m_end_command_allocator.Get(),
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "end of drawing");
        m_xfer_command_list =
            create_dx12_command_list(m_xfer_command_allocator.Get(),
                                     D3D12_COMMAND_LIST_TYPE_DIRECT,
                                     "resource transfer");
        wait_for_command_queue();
    }

    void render_context::initialize()
    {
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create swap chain";
        auto swap_chain =
            std::make_shared<mge::dx12::swap_chain>(m_render_system, *this);
        m_swap_chain = swap_chain;
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create descriptor heap";
        create_descriptor_heap();
        MGE_DEBUG_TRACE_STREAM(DX12) << "Update render target views";
        update_render_target_views(swap_chain);
        MGE_DEBUG_TRACE_STREAM(DX12) << "Create direct command lists";
        create_command_lists();
    }

    render_context::~render_context()
    {
        if (m_info_queue && m_callback_cookie != 0) {
            m_info_queue->UnregisterMessageCallback(m_callback_cookie);
        }
    }

    mge::index_buffer_ref render_context::create_index_buffer(mge::data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        mge::index_buffer_ref ref =
            std::make_shared<dx12::index_buffer>(*this, dt, data_size, data);
        return ref;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref ref =
            std::make_shared<dx12::vertex_buffer>(*this,
                                                  layout,
                                                  data_size,
                                                  data);
        return ref;
    }

    mge::shader_ref render_context::create_shader(shader_type t)
    {
        mge::shader_ref ref = std::make_shared<dx12::shader>(*this, t);
        return ref;
    }

    mge::program_ref render_context::create_program()
    {
        mge::program_ref result = std::make_shared<dx12::program>(*this);
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        mge::command_list_ref result =
            std::make_shared<dx12::command_list>(*this);
        return result;
    }

    mge::frame_command_list_ref
    render_context::create_current_frame_command_list()
    {
        switch (m_draw_state) {
        case draw_state::NONE:
            begin_draw();
            m_begin_command_list->Close();
            m_frame_command_lists.push_back(m_begin_command_list.Get());
            break;
        case draw_state::DRAW:
            break;
        case draw_state::SUBMIT:
            MGE_THROW(error) << "Invalid draw state for frame command list: "
                             << m_draw_state;
        }

        mge::frame_command_list_ref result =
            std::make_shared<dx12::frame_command_list>(
                *this,
                current_back_buffer_index());
        return result;
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

    uint32_t render_context::current_back_buffer_index() const
    {
        return m_swap_chain->current_back_buffer_index();
    }

    void render_context::wait_for_command_queue()
    {
        // wait for frame
        auto fence = m_command_queue_fence_value++;
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
        // MGE_DEBUG_TRACE(DX12)
        //     << "Frame completed: " << m_command_queue_fence_value;
    }

    void render_context::clear_frame_resources()
    {
        m_frame_resources.clear();
        m_frame_command_lists.clear();
    }

    void render_context::reset_draw()
    {
        auto rc = m_begin_command_allocator->Reset();
        CHECK_HRESULT(rc, ID3D12CommandAllocator, Reset);
        rc = m_begin_command_list->Reset(m_begin_command_allocator.Get(),
                                         nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);

        rc = m_end_command_allocator->Reset();
        CHECK_HRESULT(rc, ID3D12GraphicsCommandAllocator, Reset);
        rc = m_end_command_list->Reset(m_end_command_allocator.Get(), nullptr);
        CHECK_HRESULT(rc, ID3D12GraphicsCommandList, Reset);
    }

    void render_context::before_present()
    {
        if (m_draw_state == draw_state::NONE) {
            begin_draw();
        }
        end_draw();
        execute_frame_commands();
        m_draw_state = draw_state::SUBMIT;
    }

    void render_context::after_present()
    {
        wait_for_command_queue();
        reset_draw();
        clear_frame_resources();
        m_draw_state = draw_state::NONE;
    }

    void render_context::execute_frame_commands()
    {
        m_command_queue->ExecuteCommandLists(
            static_cast<UINT>(m_frame_command_lists.size()),
            m_frame_command_lists.data());
    }

    void render_context::end_draw()
    {
        D3D12_RESOURCE_BARRIER render_to_present = {
            .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
            .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
            .Transition = {
                .pResource = m_backbuffers[dx12_swap_chain(*m_swap_chain)
                                               .current_back_buffer_index()]
                                 .Get(),
                .Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                .StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET,
                .StateAfter = D3D12_RESOURCE_STATE_PRESENT}};
        m_end_command_list->ResourceBarrier(1, &render_to_present);
        m_end_command_list->Close();
        m_frame_command_lists.push_back(m_end_command_list.Get());
    }

    void render_context::begin_draw()
    {
        uint32_t current_buffer_index = current_back_buffer_index();
        // no wait in the beginning
        // setup draw
        D3D12_RESOURCE_BARRIER present_to_render = {
            .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
            .Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE,
            .Transition = {.pResource =
                               m_backbuffers[current_buffer_index].Get(),
                           .Subresource =
                               D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
                           .StateBefore = D3D12_RESOURCE_STATE_PRESENT,
                           .StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET},
        };
        m_begin_command_list->ResourceBarrier(1, &present_to_render);

        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle =
            m_rtv_heap->GetCPUDescriptorHandleForHeapStart();
        rtv_handle.ptr += m_rtv_descriptor_size * current_buffer_index;

        m_begin_command_list->OMSetRenderTargets(1,
                                                 &rtv_handle,
                                                 FALSE,
                                                 nullptr);
        m_begin_command_list->RSSetViewports(1, &m_viewport);
        m_begin_command_list->RSSetScissorRects(1, &m_scissor_rect);

        m_begin_command_list->Close();
        m_frame_command_lists.push_back(m_begin_command_list.Get());
        m_draw_state = draw_state::DRAW;
    }

    mge::texture_ref render_context::create_texture(texture_type type)
    {
        mge::texture_ref result;
        return result;
    }

    mge::rectangle render_context::default_scissor() const
    {
        return mge::rectangle(m_scissor_rect.left,
                              m_scissor_rect.top,
                              m_scissor_rect.right,
                              m_scissor_rect.bottom);
    }

    void render_context::enable_debug_layer()
    {
        if (m_render_system.debug()) {
            MGE_DEBUG_TRACE_STREAM(DX12) << "Enable debug layer";
            auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug));
            CHECK_HRESULT(hr, , D3D12GetDebugInterface);
            m_debug->EnableDebugLayer();
        }
    }

    void render_context::enable_debug_messages()
    {
        if (m_render_system.debug()) {
            MGE_DEBUG_TRACE_STREAM(DX12) << "Enabling debug messages";
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

} // namespace mge::dx12
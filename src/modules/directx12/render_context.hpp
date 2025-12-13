// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "error.hpp"
#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

namespace mge::dx12 {
    class window;
    class render_system;
    class swap_chain;
    class command_list;
    class frame_command_list;
    class render_context : public mge::render_context
    {
    public:
        static constexpr uint32_t buffer_count = 2;

        render_context(render_system& render_system, window& window_);
        virtual ~render_context();

        void initialize();

        mge::index_buffer* create_index_buffer(mge::data_type dt,
                                               size_t         data_size,
                                               void*          data) override;

        mge::vertex_buffer_ref
        create_vertex_buffer(const mge::vertex_layout& layout,
                             size_t                    data_size,
                             void*                     data) override;

        mge::shader_ref       create_shader(shader_type t) override;
        mge::program_ref      create_program() override;
        mge::command_list_ref create_command_list() override;
        mge::frame_command_list_ref
                         create_current_frame_command_list() override;
        mge::texture_ref create_texture(texture_type type) override;

        const mge::dx12::window& window() const
        {
            return m_window;
        }

        const mge::com_ptr<IDXGIFactory4>& factory() const
        {
            return m_factory;
        }

        const mge::com_ptr<ID3D12CommandQueue>& command_queue() const
        {
            return m_command_queue;
        }

        ID3D12Device2* device() const
        {
            return m_device.Get();
        }
        void copy_resource_sync(ID3D12Resource*       dst,
                                ID3D12Resource*       src,
                                D3D12_RESOURCE_STATES state_after);

        const mge::com_ptr<ID3D12Resource>& backbuffer(uint32_t index) const
        {
            return m_backbuffers[index];
        }

        const D3D12_VIEWPORT& viewport() const
        {
            return m_viewport;
        }
        const D3D12_RECT& scissor_rect() const
        {
            return m_scissor_rect;
        }

        mge::rectangle              default_scissor() const;
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle(uint32_t index) const;
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle(uint32_t index) const;

        void wait_for_command_queue();

        mge::com_ptr<ID3D12CommandAllocator>
        create_command_allocator(D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpos = nullptr);
        mge::com_ptr<ID3D12GraphicsCommandList>
        create_dx12_command_list(ID3D12CommandAllocator* allocator,
                                 D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpose = nullptr);

        template <typename T>
        void add_frame_resource(const mge::com_ptr<T>& resource)
        {
            mge::com_ptr<IUnknown> unknown;
            auto                   rc = resource.As(&unknown);
            if (FAILED(rc)) {
                MGE_THROW(dx12::error) << "Failed to add frame resource";
            }
            m_frame_resources.push_back(unknown);
        }

        void execute_dx12_command_list(ID3D12CommandList* list)
        {
            m_frame_command_lists.push_back(list);
        }

        void before_present();
        void after_present();

    private:
        friend class swap_chain;

        void enable_debug_layer();
        void create_factory();
        void create_adapter();
        void create_device();
        void enable_debug_messages();
        void create_command_queue();

        void create_descriptor_heap();
        void update_render_target_views(
            const std::shared_ptr<mge::dx12::swap_chain>& swap_chain);

        void     create_command_lists();
        uint32_t current_back_buffer_index() const;

        void tmp_draw();

        void begin_draw();
        void end_draw();
        void execute_frame_commands();
        void clear_frame_resources();
        void reset_draw();

        static void message_func(D3D12_MESSAGE_CATEGORY category,
                                 D3D12_MESSAGE_SEVERITY severity,
                                 D3D12_MESSAGE_ID       id,
                                 LPCSTR                 description,
                                 void*                  context);

        render_system&     m_render_system;
        mge::dx12::window& m_window;

        mge::com_ptr<ID3D12Debug>                 m_debug;
        mge::com_ptr<IDXGIFactory4>               m_factory;
        mge::com_ptr<IDXGIAdapter4>               m_adapter;
        mge::com_ptr<ID3D12Device2>               m_device;
        mge::com_ptr<ID3D12InfoQueue1>            m_info_queue;
        mge::com_ptr<ID3D12CommandQueue>          m_command_queue;
        mge::com_ptr<ID3D12Fence>                 m_command_queue_fence;
        uint64_t                                  m_command_queue_fence_value;
        HANDLE                                    m_command_queue_fence_event;
        mge::com_ptr<ID3D12DescriptorHeap>        m_rtv_heap;
        mge::com_ptr<ID3D12DescriptorHeap>        m_dsv_heap;
        std::vector<mge::com_ptr<ID3D12Resource>> m_backbuffers;

        mge::com_ptr<ID3D12CommandAllocator>    m_begin_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_begin_command_list;

        mge::com_ptr<ID3D12CommandAllocator>    m_end_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_end_command_list;

        mge::com_ptr<ID3D12CommandAllocator>    m_xfer_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_xfer_command_list;

        D3D12_VIEWPORT m_viewport;
        D3D12_RECT     m_scissor_rect;
        uint32_t       m_rtv_descriptor_size;
        uint32_t       m_dsv_descriptor_size;
        DWORD          m_callback_cookie;

        std::vector<mge::com_ptr<IUnknown>> m_frame_resources;
        std::vector<ID3D12CommandList*>     m_frame_command_lists;

        enum class draw_state
        {
            NONE,
            DRAW,
            SUBMIT,
        };

        draw_state m_draw_state{draw_state::NONE};
        mge::mutex m_data_lock;
        std::vector<std::unique_ptr<mge::index_buffer>> m_index_buffers;
    };

    inline render_context& dx12_context(mge::render_context& context)
    {
        return static_cast<render_context&>(context);
    }

    inline const render_context&
    dx12_context(const mge::render_context& context)
    {
        return static_cast<const render_context&>(context);
    }

} // namespace mge::dx12

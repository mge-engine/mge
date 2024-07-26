// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

namespace mge::dx12 {
    class window;
    class render_system;
    class swap_chain;
    class command_list;

    class render_context : public mge::render_context
    {
    public:
        static constexpr uint32_t buffer_count = 2;

        render_context(render_system& render_system, window& window_);
        virtual ~render_context();

        void initialize();

        mge::index_buffer_ref create_index_buffer(mge::data_type dt,
                                                  size_t         data_size,
                                                  void*          data) override;

        mge::vertex_buffer_ref
        create_vertex_buffer(const mge::vertex_layout& layout,
                             size_t                    data_size,
                             void*                     data) override;

        mge::shader_ref       create_shader(shader_type t) override;
        mge::program_ref      create_program() override;
        mge::command_list_ref create_command_list() override;
        mge::texture_ref      create_texture(texture_type type) override;

        const mge::dx12::window& window() const { return m_window; }

        const mge::com_ptr<IDXGIFactory4>& factory() const { return m_factory; }

        const mge::com_ptr<ID3D12CommandQueue>& command_queue() const
        {
            return m_command_queue;
        }

        ID3D12Device2* device() const { return m_device.Get(); }
        void           copy_resource(ID3D12Resource*       dst,
                                     ID3D12Resource*       src,
                                     D3D12_RESOURCE_STATES state_after);

        mge::com_ptr<ID3D12CommandAllocator> get_command_allocator();

        void release_command_allocator(
            mge::com_ptr<ID3D12CommandAllocator>& allocator);

    private:
        friend class swap_chain;

        void create_factory();
        void create_adapter();
        void create_device();
        void enable_debug_messages();
        void create_command_queue();
        void create_direct_command_list();

        void create_descriptor_heap();
        void update_render_target_views(
            const std::shared_ptr<mge::dx12::swap_chain>& swap_chain);
        void reset_direct_command_list();
        //  void begin_draw();
        //  void end_draw();
        void wait_for_command_queue();

        static void message_func(D3D12_MESSAGE_CATEGORY category,
                                 D3D12_MESSAGE_SEVERITY severity,
                                 D3D12_MESSAGE_ID       id,
                                 LPCSTR                 description,
                                 void*                  context);

        render_system&     m_render_system;
        mge::dx12::window& m_window;

        mge::com_ptr<IDXGIFactory4>               m_factory;
        mge::com_ptr<IDXGIAdapter4>               m_adapter;
        mge::com_ptr<ID3D12Device2>               m_device;
        mge::com_ptr<ID3D12InfoQueue1>            m_info_queue;
        mge::com_ptr<ID3D12CommandQueue>          m_command_queue;
        mge::com_ptr<ID3D12Fence>                 m_command_queue_fence;
        uint64_t                                  m_command_queue_fence_value;
        HANDLE                                    m_command_queue_fence_event;
        mge::com_ptr<ID3D12DescriptorHeap>        m_rtv_heap;
        std::vector<mge::com_ptr<ID3D12Resource>> m_backbuffers;

        mge::com_ptr<ID3D12CommandAllocator>    m_command_list_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_direct_command_list;
        D3D12_VIEWPORT                          m_viewport;
        D3D12_RECT                              m_scissor_rect;
        uint32_t                                m_rtv_descriptor_size;
        DWORD                                   m_callback_cookie;
        bool                                    m_drawing;
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

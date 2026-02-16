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
#include "mge/graphics/uniform_block.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

#include <map>
#include <tuple>
#include <utility>

namespace mge::dx12 {
    class window;
    class render_system;
    class program;

    class render_context : public mge::render_context
    {
    public:
        static constexpr uint32_t buffer_count = 2;

        render_context(render_system& render_system, window& window_);
        virtual ~render_context();

        void initialize();

        mge::index_buffer* on_create_index_buffer(mge::data_type dt,
                                                  size_t data_size) override;

        mge::vertex_buffer*
        on_create_vertex_buffer(const mge::vertex_layout& layout,
                                size_t                    data_size) override;

        mge::shader*  on_create_shader(shader_type t) override;
        mge::program* on_create_program() override;
        void          on_frame_present() override;

        mge::texture_ref create_texture(texture_type type) override;
        mge::image_ref   screenshot() override;

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

        void
        copy_texture_sync(ID3D12Resource*                           dst,
                          ID3D12Resource*                           src,
                          const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& footprint);

        std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE>
        allocate_srv();

        ID3D12DescriptorHeap* srv_heap() const
        {
            return m_srv_heap.Get();
        }

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

        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle(uint32_t index) const;
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle(uint32_t index) const;

        void wait_for_command_queue();

        mge::com_ptr<ID3D12CommandAllocator>
        create_command_allocator(D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpos = nullptr);
        mge::com_ptr<ID3D12GraphicsCommandList>
        create_dx12_command_list(ID3D12CommandAllocator* allocator,
                                 D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpose = nullptr,
                                 bool                    reset = true);

        void render(const mge::pass& p) override;

        void remove_pipeline_state(mge::dx12::program* program)
        {
            std::lock_guard<mge::mutex> lock(m_data_lock);
            auto min_key = std::make_tuple(static_cast<void*>(program),
                                           mge::pipeline_state{});
            auto it = m_program_pipeline_states.lower_bound(min_key);
            while (it != m_program_pipeline_states.end() &&
                   std::get<0>(it->first) == program) {
                it = m_program_pipeline_states.erase(it);
            }
        }

        const mge::com_ptr<ID3D12PipelineState>&
        static_pipeline_state(mge::dx12::program*        program,
                              const mge::pipeline_state& state);

    private:
        void enable_debug_layer();
        void create_factory();
        void create_adapter();
        void create_device();
        void enable_debug_messages();
        void create_command_queue();
        void create_descriptor_heap();
        void create_swap_chain();
        void update_render_target_views();
        void create_depth_stencil_views();
        void create_command_lists();

        void draw_geometry(ID3D12GraphicsCommandList* command_list,
                           mge::program*              program,
                           mge::vertex_buffer*        vb,
                           mge::index_buffer*         ib,
                           const mge::pipeline_state& state,
                           mge::uniform_block*        ub,
                           mge::texture*              tex,
                           uint32_t                   index_count = 0,
                           uint32_t                   index_offset = 0);

        void bind_uniform_block(ID3D12GraphicsCommandList* command_list,
                                mge::dx12::program&        dx12_program,
                                mge::uniform_block&        ub);

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
        mge::com_ptr<ID3D12DescriptorHeap>        m_srv_heap;
        std::vector<mge::com_ptr<ID3D12Resource>> m_backbuffers;
        std::vector<mge::com_ptr<ID3D12Resource>> m_depth_buffers;
        mge::com_ptr<IDXGISwapChain4>             m_swap_chain;

        mge::com_ptr<ID3D12CommandAllocator>    m_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_command_list;

        mge::com_ptr<ID3D12CommandAllocator>    m_xfer_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_xfer_command_list;

        using pipeline_state_key = std::tuple<void*, mge::pipeline_state>;

        std::map<pipeline_state_key, mge::com_ptr<ID3D12PipelineState>>
            m_program_pipeline_states;

        D3D12_VIEWPORT m_viewport;
        D3D12_RECT     m_scissor_rect;
        uint32_t       m_rtv_descriptor_size;
        uint32_t       m_dsv_descriptor_size;
        uint32_t       m_srv_descriptor_size;
        uint32_t       m_srv_next_index;
        DWORD          m_callback_cookie;

        D3D12_RASTERIZER_DESC m_rasterizer_desc;
        D3D12_BLEND_DESC      m_blend_desc_no_blend;

        enum class draw_state
        {
            NONE,
            DRAW,
            SUBMIT,
        };

        draw_state m_draw_state{draw_state::NONE};

        std::map<mge::uniform_block*, mge::com_ptr<ID3D12Resource>>
                                                m_constant_buffers;
        std::map<mge::uniform_block*, uint64_t> m_constant_buffer_versions;
        mge::mutex                              m_data_lock;
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

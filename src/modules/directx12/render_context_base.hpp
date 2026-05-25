// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "error.hpp"
#include "input_layout_cache.hpp"
#include "mge/core/mutex.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "mge/win32/com_ptr.hpp"

#include <map>
#include <tuple>
#include <utility>

namespace mge::dx12 {
    class render_system;
    class program;

    class render_context_base : public mge::render_context
    {
    public:
        static constexpr uint32_t buffer_count   = 2;
        static constexpr uint32_t max_extra_rtvs = 64;
        static constexpr uint32_t max_extra_dsvs = 64;

        virtual ~render_context_base();

        mge::index_buffer* on_create_index_buffer(mge::data_type dt,
                                                  size_t data_size) override;
        mge::vertex_buffer*
        on_create_vertex_buffer(const mge::vertex_layout& layout,
                                size_t                    data_size) override;
        mge::shader*       on_create_shader(shader_type t) override;
        mge::program*      on_create_program() override;
        mge::frame_buffer* on_create_frame_buffer(
            const mge::frame_buffer_info& info) override;

        mge::texture_ref create_texture(texture_type type) override;
        mge::texture_ref
        create_render_target_texture(texture_type        type,
                                     const image_format& format,
                                     const mge::extent&  extent) override;

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

        const D3D12_VIEWPORT& viewport() const
        {
            return m_viewport;
        }
        const D3D12_RECT& scissor_rect() const
        {
            return m_scissor_rect;
        }

        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle(uint32_t index) const;
        D3D12_CPU_DESCRIPTOR_HANDLE allocate_rtv();
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle(uint32_t index) const;
        D3D12_CPU_DESCRIPTOR_HANDLE allocate_dsv();

        void wait_for_command_queue();

        mge::com_ptr<ID3D12CommandAllocator>
        create_command_allocator(D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpose = nullptr);
        mge::com_ptr<ID3D12GraphicsCommandList>
        create_dx12_command_list(ID3D12CommandAllocator* allocator,
                                 D3D12_COMMAND_LIST_TYPE type,
                                 const char*             purpose = nullptr,
                                 bool                    reset   = true);

        void render(const mge::pass& p) override;

        void remove_pipeline_state(mge::dx12::program* program)
        {
            std::lock_guard<mge::mutex> lock(m_data_lock);
            auto min_key = std::make_tuple(static_cast<void*>(program),
                                           mge::pipeline_state{},
                                           DXGI_FORMAT_UNKNOWN,
                                           DXGI_FORMAT_UNKNOWN);
            auto it = m_program_pipeline_states.lower_bound(min_key);
            while (it != m_program_pipeline_states.end() &&
                   std::get<0>(it->first) == program) {
                it = m_program_pipeline_states.erase(it);
            }
        }

        const mge::com_ptr<ID3D12PipelineState>& static_pipeline_state(
            mge::dx12::program*                          program,
            const mge::pipeline_state&                   state,
            const std::vector<D3D12_INPUT_ELEMENT_DESC>& input_layout,
            DXGI_FORMAT rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_FORMAT dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT);

    protected:
        render_context_base(mge::dx12::render_system& system,
                            const mge::extent&        ext);

        void enable_debug_layer();
        void create_factory();
        void create_adapter();
        void create_device();
        void init_capabilities();
        void enable_debug_messages();
        void create_command_queue();
        void create_descriptor_heap();
        void create_command_lists();
        void ensure_command_list_open();

        virtual void setup_backbuffer_as_render_target(
            ID3D12GraphicsCommandList*   cmd_list,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_rtv,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_dsv,
            bool&                        pass_has_dsv,
            DXGI_FORMAT&                 pass_rtv_format,
            DXGI_FORMAT&                 pass_dsv_format);

        void draw_geometry(ID3D12GraphicsCommandList*       command_list,
                           mge::program*                    program,
                           mge::vertex_buffer*              vb,
                           mge::index_buffer*               ib,
                           const mge::pipeline_state&       state,
                           mge::uniform_block*              ub,
                           const mge::texture_binding_list& textures,
                           DXGI_FORMAT                      rtv_format,
                           DXGI_FORMAT                      dsv_format,
                           uint32_t                         index_count  = 0,
                           uint32_t                         index_offset = 0);

        const std::vector<D3D12_INPUT_ELEMENT_DESC>&
        input_layout_from_vertex_buffer(mge::vertex_buffer* vb);

        void bind_uniform_block(ID3D12GraphicsCommandList* command_list,
                                mge::dx12::program&        dx12_program,
                                mge::uniform_block&        ub);

        static void message_func(D3D12_MESSAGE_CATEGORY category,
                                 D3D12_MESSAGE_SEVERITY severity,
                                 D3D12_MESSAGE_ID       id,
                                 LPCSTR                 description,
                                 void*                  context);

        enum class draw_state
        {
            NONE,
            FBO_DRAW,
            DRAW,
            SUBMIT,
        };

        mge::dx12::render_system& m_render_system;

        mge::com_ptr<ID3D12Debug>      m_debug;
        mge::com_ptr<IDXGIFactory4>    m_factory;
        mge::com_ptr<IDXGIAdapter4>    m_adapter;
        mge::com_ptr<ID3D12Device2>    m_device;
        mge::com_ptr<ID3D12InfoQueue1> m_info_queue;
        mge::com_ptr<ID3D12CommandQueue> m_command_queue;
        mge::com_ptr<ID3D12Fence>        m_command_queue_fence;
        uint64_t                         m_command_queue_fence_value{0};
        HANDLE                           m_command_queue_fence_event{0};
        mge::com_ptr<ID3D12DescriptorHeap> m_rtv_heap;
        mge::com_ptr<ID3D12DescriptorHeap> m_dsv_heap;
        mge::com_ptr<ID3D12DescriptorHeap> m_srv_heap;

        mge::com_ptr<ID3D12CommandAllocator>    m_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_command_list;
        mge::com_ptr<ID3D12CommandAllocator>    m_xfer_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_xfer_command_list;

        using pipeline_state_key =
            std::tuple<void*, mge::pipeline_state, DXGI_FORMAT, DXGI_FORMAT>;
        std::map<pipeline_state_key, mge::com_ptr<ID3D12PipelineState>>
            m_program_pipeline_states;

        input_layout_cache m_input_layout_cache;

        D3D12_VIEWPORT m_viewport{};
        D3D12_RECT     m_scissor_rect{};

        uint32_t m_rtv_descriptor_size{0};
        uint32_t m_rtv_next_index{0};
        uint32_t m_dsv_descriptor_size{0};
        uint32_t m_dsv_next_index{0};
        uint32_t m_srv_descriptor_size{0};
        uint32_t m_srv_next_index{0};
        DWORD    m_callback_cookie{0};

        D3D12_RASTERIZER_DESC m_rasterizer_desc{};
        D3D12_BLEND_DESC      m_blend_desc_no_blend{};

        draw_state m_draw_state{draw_state::NONE};

        std::map<mge::uniform_block*, mge::com_ptr<ID3D12Resource>>
                                                m_constant_buffers;
        std::map<mge::uniform_block*, uint64_t> m_constant_buffer_versions;
        mge::mutex                              m_data_lock;
    };

    inline render_context_base& dx12_context(mge::render_context& context)
    {
        return dynamic_cast<render_context_base&>(context);
    }

    inline const render_context_base&
    dx12_context(const mge::render_context& context)
    {
        return dynamic_cast<const render_context_base&>(context);
    }

} // namespace mge::dx12

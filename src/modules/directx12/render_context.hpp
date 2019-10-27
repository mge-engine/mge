// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/memory_command_list.hpp"
#include "win32/com_unique_ptr.hpp"
#include "win32/com_ptr.hpp"

namespace dx12 {
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(window *win,
                       const system_config& config);
        ~render_context();

        void flush() override;
        mge::vertex_buffer_ref create_vertex_buffer(const mge::vertex_layout& layout,
                                            mge::usage usage,
                                            size_t element_count,
                                            void *initial_data) override;
        mge::index_buffer_ref create_index_buffer(mge::data_type type,
                                                  mge::usage usage,
                                                  size_t element_count,
                                                  void *initial_data) override;
        mge::texture_2d_ref create_texture_2d(const mge::image_ref& image) override;
        mge::texture_2d_ref create_texture_2d() override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        mge::command_list_ref create_command_list() override;
        void execute(const mge::command_list_ref& commands) override;
        mge::pipeline_ref create_pipeline() override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;

    private:
        uint32_t frame_count() const { return 2; }
        void select_adapter(IDXGIFactory4 *factory,
                            IDXGIAdapter1** adapter);
        void create_command_queue();
        void create_swap_chain(IDXGIFactory4 *factory, window *win);
        void create_descriptor_heap();
        void create_frame_resources();
        void create_command_allocator();
        void create_graphics_command_list();
        void create_sync_objects();
        void wait_for_frame();
        void materialize_commands();

        mge::com_ptr<ID3D12Device>                m_device;
        mge::com_ptr<ID3D12CommandQueue>          m_command_queue;
        mge::com_ptr<IDXGISwapChain3>             m_swap_chain;
        mge::com_ptr<ID3D12DescriptorHeap>        m_rtv_heap;
        std::vector<mge::com_ptr<ID3D12Resource>> m_render_targets;
        mge::com_ptr<ID3D12CommandAllocator>      m_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList>   m_command_list;
        mge::com_ptr<ID3D12PipelineState>         m_pipeline_state;
        mge::com_ptr<ID3D12Fence>                 m_fence;
        std::shared_ptr<mge::memory_command_list> m_commands;
        HANDLE                                    m_fence_event;
        uint64_t                                  m_fence_value;
        uint32_t                                  m_frame_index;
        uint32_t                                  m_rtv_descriptor_size;
    };

}

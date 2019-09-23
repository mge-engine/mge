// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_context.hpp"
#include "win32/com_unique_ptr.hpp"

namespace dx12 {
    class window;

    class render_context : public mge::render_context
    {
    public:
        static constexpr size_t RENDER_TARGET_COUNT = 2;

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

        ID3D12Device *device() const { return m_device.get(); }
        ID3D12CommandAllocator *command_allocator() const { return m_command_allocator.get(); }
    private:
        void init_context(const system_config& config);
        void create_factory();
        void select_adapter();
        bool setup_device();
        void create_command_queue();
        void create_swap_chain();
        void disable_fullscreen_transition();
        void create_descriptor_heap();
        void create_render_target_views();
        void create_command_allocator();
        void create_fence();

        window *m_window;

        COM_UNIQUE_PTR(IDXGIFactory4)          m_dxgi_factory;
        COM_UNIQUE_PTR(IDXGIAdapter3)          m_adapter;
        COM_UNIQUE_PTR(ID3D12Device)           m_device;
        COM_UNIQUE_PTR(ID3D12CommandQueue)     m_command_queue;
        COM_UNIQUE_PTR(IDXGISwapChain4)        m_swap_chain;
        COM_UNIQUE_PTR(ID3D12DescriptorHeap)   m_rtv_heap;
        COM_UNIQUE_PTR(ID3D12Resource)         m_buffers[RENDER_TARGET_COUNT];
        COM_UNIQUE_PTR(ID3D12CommandAllocator) m_command_allocator;
        COM_UNIQUE_PTR(ID3D12Fence)            m_fence;
        HANDLE                                 m_event;

        D3D_FEATURE_LEVEL m_feature_level;
        UINT              m_frame_index;
        UINT              m_rtv_descriptor_size;
        uint64_t          m_fence_value;
    };

    inline ID3D12Device *dx12_device(const mge::render_context& context)
    {
        return static_cast<const render_context&>(context).device();
    }

    inline ID3D12CommandAllocator *dx12_command_allocator(const mge::render_context& context)
    {
        return static_cast<const render_context&>(context).command_allocator();
    }
}

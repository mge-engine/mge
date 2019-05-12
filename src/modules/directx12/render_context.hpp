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
        static const size_t RENDER_TARGET_COUNT = 2;

        render_context(window *win,
                       const system_config& config);
        ~render_context();

        void flush() override;
        mge::index_buffer_ref create_index_buffer(mge::buffer_usage usage,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access) override;
        mge::index_buffer_ref create_index_buffer(mge::buffer_usage usage,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access,
                                                  void *data,
                                                  size_t size) override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;
    private:
        void init_context(const system_config& config);
        void create_factory();
        void select_adapter();
        bool setup_device();
        void create_command_queue();
        void create_swap_chain();

        window *m_window;

        COM_UNIQUE_PTR(IDXGIFactory4)          m_dxgi_factory;
        COM_UNIQUE_PTR(IDXGIAdapter3)          m_adapter;
        COM_UNIQUE_PTR(ID3D12Device)           m_device;
        COM_UNIQUE_PTR(IDXGISwapChain3)        m_swap_chain;
        COM_UNIQUE_PTR(ID3D12CommandQueue)     m_command_queue;
        COM_UNIQUE_PTR(ID3D12DescriptorHeap)   m_rtv_heap;
        COM_UNIQUE_PTR(ID3D12Resource)         m_render_targets[RENDER_TARGET_COUNT];
        COM_UNIQUE_PTR(ID3D12CommandAllocator) m_command_allocator;

        D3D_FEATURE_LEVEL m_feature_level;
        UINT              m_frame_index;
        UINT              m_rtv_descriptor_size;

    };
}

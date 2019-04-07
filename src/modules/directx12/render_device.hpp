// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "moge/render/render_device.hpp"
#include "dx12.hpp"
#include "system_config.hpp"



namespace dx12 {
    class window;

    class render_device : public moge::render_device
    {
    public:
        static const size_t FRAME_COUNT = 2;

        render_device(window *w, const system_config& config);
        virtual ~render_device();
        moge::swap_chain_ref swap_chain() const;

    private:
        void init();
        void setup_device(IDXGIAdapter3 *adapter);

        window *m_window;

        COM_UNIQUE_PTR(IDXGIFactory4)          m_dxgi_factory;
        COM_UNIQUE_PTR(IDXGIAdapter3)          m_adapter;
        COM_UNIQUE_PTR(ID3D12Device)           m_device;
        COM_UNIQUE_PTR(IDXGISwapChain3)        m_swap_chain;
        COM_UNIQUE_PTR(ID3D12CommandQueue)     m_command_queue;
        COM_UNIQUE_PTR(ID3D12DescriptorHeap)   m_rtv_heap;
        COM_UNIQUE_PTR(ID3D12Resource)         m_render_targets[FRAME_COUNT];
        COM_UNIQUE_PTR(ID3D12CommandAllocator) m_command_allocator;

        D3D_FEATURE_LEVEL              m_feature_level;
        UINT                           m_frame_index;
        UINT                           m_rtv_descriptor_size;
    };
}

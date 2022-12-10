// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx12.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/win32/com_ptr.hpp"
#include "window.hpp"

namespace mge::dx12 {
    class window;
    class render_system;
    class swap_chain;

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

        const mge::dx12::window& window() const { return m_window; }

        const mge::com_ptr<IDXGIFactory4>& factory() const { return m_factory; }

        const mge::com_ptr<ID3D12CommandQueue>& command_queue() const
        {
            return m_command_queue;
        }

    private:
        void create_factory();
        void create_adapter();
        void create_device();
        void enable_debug_messages();
        void create_command_queue();
        void create_descriptor_heap();
        void update_render_target_views(
            const std::shared_ptr<mge::dx12::swap_chain>& swap_chain);

        render_system&     m_render_system;
        mge::dx12::window& m_window;

        mge::com_ptr<IDXGIFactory4>               m_factory;
        mge::com_ptr<IDXGIAdapter4>               m_adapter;
        mge::com_ptr<ID3D12Device2>               m_device;
        mge::com_ptr<ID3D12CommandQueue>          m_command_queue;
        mge::com_ptr<ID3D12DescriptorHeap>        m_rtv_heap;
        std::vector<mge::com_ptr<ID3D12Resource>> m_backbuffers;
        uint32_t                                  m_rtv_descriptor_size;
    };

} // namespace mge::dx12

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
        void select_adapter(IDXGIFactory4 *factory,
                            IDXGIAdapter1** adapter);
        void create_command_queue();

        COM_PTR(ID3D12Device)       m_device;
        COM_PTR(ID3D12CommandQueue) m_command_queue;
    };

}

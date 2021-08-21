// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/graphics/render_context.hpp"
#include "window.hpp"

namespace mge::dx12 {
    class window;
    class render_system;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& render_system, window& window_);
        virtual ~render_context();

        mge::index_buffer_ref create_index_buffer(mge::data_type dt,
                                                  size_t         data_size,
                                                  void*          data) override;

        mge::vertex_buffer_ref
        create_vertex_buffer(const mge::vertex_layout& layout,
                             size_t                    data_size,
                             void*                     data) override;

        mge::shader_ref create_shader(shader_type t) override;

    private:
        render_system& m_render_system;
        window&        m_window;
    };
} // namespace mge::dx12

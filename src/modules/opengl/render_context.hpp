// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl.hpp"
#include "window.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/render_context.hpp"

namespace opengl {
    class render_context : public mge::render_context
    {
    public:
        render_context(window *w);
        ~render_context();
        mge::vertex_buffer_ref create_vertex_buffer(const mge::vertex_layout& layout,
                                                    mge::usage usage,
                                                    mge::buffer_access cpu_access,
                                                    mge::buffer_access gpu_access,
                                                    size_t element_count,
                                                     void *initial_data) override;

        mge::index_buffer_ref create_index_buffer(mge::data_type type,
                                                  mge::usage usage,
                                                  mge::buffer_access cpu_access,
                                                  mge::buffer_access gpu_access,
                                                  size_t element_count,
                                                  void *initial_data) override;
        mge::texture_2d_ref create_texture_2d(mge::usage texture_usage,
                                              mge::filter_function mag_filter,
                                              mge::filter_function min_filter,
                                              bool mipmap_use) override;
        mge::shader_ref create_shader(mge::shader_type type) override;
        mge::pipeline_ref create_pipeline() override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;
#ifdef MGE_OS_WINDOWS
        void assign_thread() override;
        void flush() override;
    private:
        void choose_pixel_format();
        void create_glrc();
        void init_gl3w();
        void collect_opengl_info();
        void clear_current();

        HWND  m_hwnd;
        HDC   m_hdc;
        HGLRC m_hglrc;
#endif
    };
}

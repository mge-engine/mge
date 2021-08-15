// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/singleton.hpp"
#include "mge/graphics/render_context.hpp"
#include "opengl.hpp"
#include "opengl_info.hpp"
#include "window.hpp"

namespace mge {

    namespace opengl {
        class render_context : public mge::render_context
        {
        public:
            render_context(mge::opengl::window* context_window);
            virtual ~render_context();

            mge::index_buffer_ref create_index_buffer(mge::data_type dt,
                                                      size_t         data_size,
                                                      void* data) override;

            mge::vertex_buffer_ref
            create_vertex_buffer(const mge::vertex_layout& layout,
                                 size_t                    data_size,
                                 void*                     data) override;

            mge::shader_ref create_shader(shader_type t) override;

#ifdef MGE_OS_WINDOWS

            HDC dc() const { return m_hdc; }

        private:
            void select_pixel_format();
            void create_glrc();
            void init_gl3w();
            void collect_opengl_info();

            HWND  m_hwnd;
            HDC   m_hdc;
            HGLRC m_hglrc;
#else
#    error Missing port
#endif
            static singleton<opengl_info> s_glinfo;
        };
    } // namespace opengl
} // namespace mge
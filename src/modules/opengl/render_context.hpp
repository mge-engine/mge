// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/thread.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "opengl.hpp"
#include "opengl_info.hpp"
#include "window.hpp"

namespace mge {

    namespace opengl {
        class render_system;

        class render_context : public mge::render_context
        {
        public:
            render_context(mge::opengl::render_system& render_system_,
                           mge::opengl::window*        context_window);
            virtual ~render_context();

            mge::index_buffer*
            on_create_index_buffer(mge::data_type dt,
                                   size_t         data_size) override;

            mge::vertex_buffer*
            on_create_vertex_buffer(const mge::vertex_layout& layout,
                                    size_t data_size) override;

            void render(const mge::pass& p) override;

            opengl_info& gl_info()
            {
                return *s_glinfo;
            }

            mge::shader*  on_create_shader(shader_type t) override;
            mge::program* on_create_program() override;
            void          on_frame_present() override;

            mge::texture_ref create_texture(texture_type type) override;
            mge::image_ref   screenshot() override;

            mge::rectangle default_scissor() const;
            uint32_t       window_height() const;
#ifdef MGE_OS_WINDOWS

            HDC dc() const
            {
                return m_hdc;
            }

        private:
            void select_pixel_format();
            void create_primary_glrc();
            void init_gl3w();
            void collect_opengl_info();

            mge::opengl::window*        m_window;
            HWND                        m_hwnd;
            HDC                         m_hdc;
            HGLRC                       m_primary_hglrc;
            mge::mutex                  m_lock;
            std::map<thread::id, HGLRC> m_thread_glrcs;
            bool                        m_record_frames{false};
            bool                        m_first_frame{true};
#else
#    error Missing port
#endif
            static singleton<opengl_info> s_glinfo;
        };
    } // namespace opengl
} // namespace mge
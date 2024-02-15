// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/lru_cache.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/thread.hpp"
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

            opengl_info& gl_info() { return *s_glinfo; }

            mge::shader_ref           create_shader(shader_type t) override;
            mge::program_ref          create_program() override;
            mge::command_list_ref     create_command_list() override;
            mge::command_sequence_ref create_command_sequence() override;

            void execute(const mge::command_sequence& sequence) override;

#ifdef MGE_OS_WINDOWS

            HDC dc() const { return m_hdc; }

            void create_swap_chain(); // needed for two-step initialization
        private:
            void select_pixel_format();
            void create_primary_glrc();
            void init_gl3w();
            void collect_opengl_info();

            HWND                        m_hwnd;
            HDC                         m_hdc;
            HGLRC                       m_primary_hglrc;
            mge::mutex                  m_lock;
            std::map<thread::id, HGLRC> m_thread_glrcs;
#else
#    error Missing port
#endif
            static singleton<opengl_info> s_glinfo;
        };
    } // namespace opengl
} // namespace mge
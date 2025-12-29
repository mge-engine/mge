// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/mutex.hpp"
#include "mge/core/singleton.hpp"
#include "mge/core/thread.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/graphics/rectangle.hpp"
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

            mge::index_buffer* create_index_buffer(mge::data_type dt,
                                                   size_t         data_size,
                                                   void* data) override;

            void destroy_index_buffer(mge::index_buffer* ib) override;

            mge::vertex_buffer*
            create_vertex_buffer(const mge::vertex_layout& layout,
                                 size_t                    data_size,
                                 void*                     data) override;

            void destroy_vertex_buffer(mge::vertex_buffer* vb) override;

            opengl_info& gl_info()
            {
                return *s_glinfo;
            }

            mge::shader*       on_create_shader(shader_type t) override;
            void               destroy_shader(mge::shader* s) override;
            mge::program*      create_program() override;
            void               destroy_program(mge::program* p) override;
            mge::command_list* create_command_list() override;
            void destroy_command_list(mge::command_list* cl) override;
            mge::frame_command_list*
            create_current_frame_command_list() override;
            void
            destroy_frame_command_list(mge::frame_command_list* fcl) override;
            mge::texture_ref create_texture(texture_type type) override;
            mge::rectangle   default_scissor() const;
            uint32_t         window_height() const;
#ifdef MGE_OS_WINDOWS

            HDC dc() const
            {
                return m_hdc;
            }

            void create_swap_chain(); // needed for two-step initialization
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
#else
#    error Missing port
#endif
            static singleton<opengl_info> s_glinfo;
            std::unordered_map<mge::index_buffer*,
                               std::unique_ptr<mge::index_buffer>>
                m_index_buffers;
            std::unordered_map<mge::vertex_buffer*,
                               std::unique_ptr<mge::vertex_buffer>>
                m_vertex_buffers;
            std::unordered_map<mge::program*, std::unique_ptr<mge::program>>
                m_programs;
            std::unordered_map<mge::command_list*,
                               std::unique_ptr<mge::command_list>>
                m_command_lists;
            std::unordered_map<mge::frame_command_list*,
                               std::unique_ptr<mge::frame_command_list>>
                m_frame_command_lists;
        };
    } // namespace opengl
} // namespace mge
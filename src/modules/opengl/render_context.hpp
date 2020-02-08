// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl.hpp"
#include "window.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/draw_command.hpp"
#include <unordered_set>
#include <string>
#include <tuple>

namespace opengl {
    class render_context : public mge::render_context
    {
    public:
        using vao_key_type = std::tuple<GLuint, GLuint, GLuint>;

        render_context(window *w, bool debug);
        ~render_context();
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
        mge::pipeline_ref create_pipeline() override;
        mge::command_list_ref create_command_list() override;
        void shader_languages(std::vector<mge::shader_language>& languages) const override;
        void execute(const mge::command_list_ref& commands) override;
#ifdef MGE_OS_WINDOWS
        void assign_thread() override;
        void flush() override;
    private:
        void draw_command(const mge::draw_command& cmd);
        void choose_pixel_format();
        void create_glrc();
        void init_gl3w();
        void collect_opengl_info();
        void install_debug_callback();
        void clear_current();
        void clear_vaos();
        GLuint lookup_vao(const mge::draw_command& cmd);

        HWND  m_hwnd;
        HDC   m_hdc;
        HGLRC m_hglrc;
        bool  m_debug;
        std::map<vao_key_type, GLuint> m_vaos;
        std::unordered_set<std::string> m_extensions;
#endif
    };
}

// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#include "mge/core/singleton.hpp"
#include "mge/graphics/rectangle.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "opengl.hpp"
#include "opengl_info.hpp"

#include <map>
#include <tuple>

namespace mge::opengl {
    class render_system;
    class vertex_buffer;
    class index_buffer;
    class program;

    class render_context_base : public mge::render_context
    {
    public:
        virtual ~render_context_base();

        mge::index_buffer*
        on_create_index_buffer(mge::data_type dt,
                               size_t         data_size) override;

        mge::vertex_buffer*
        on_create_vertex_buffer(const mge::vertex_layout& layout,
                                size_t                    data_size) override;

        mge::shader*       on_create_shader(shader_type t) override;
        mge::program*      on_create_program() override;
        mge::frame_buffer* on_create_frame_buffer(
            const mge::frame_buffer_info& info) override;

        mge::texture_ref create_texture(texture_type type) override;
        mge::texture_ref
        create_render_target_texture(texture_type        type,
                                     const image_format& format,
                                     const mge::extent&  extent) override;

        void render(const mge::pass& p) override;

        opengl_info& gl_info()
        {
            return *s_glinfo;
        }

        mge::rectangle default_scissor() const;
        uint32_t       window_height() const;

    protected:
        render_context_base(mge::opengl::render_system& render_system_,
                            const mge::extent&          ext);

        void init_gl3w();
        void collect_opengl_info();
        void init_capabilities();

        GLuint create_vao(mge::opengl::vertex_buffer* vb,
                          mge::opengl::index_buffer*  ib);

        void draw_geometry(mge::program*                    program,
                           mge::vertex_buffer*              vb,
                           mge::index_buffer*               ib,
                           mge::uniform_block*              ub,
                           const mge::texture_binding_list& textures,
                           uint32_t                         index_count  = 0,
                           uint32_t                         index_offset = 0);

        void bind_uniform_block(mge::opengl::program& gl_program,
                                mge::uniform_block&   ub);

        mge::opengl::render_system& m_render_system;

        static singleton<opengl_info> s_glinfo;
        static bool                   s_gl3w_initialized;

        bool m_conservative_rasterization_supported{false};

        using vao_key = std::tuple<GLuint, GLuint>;
        std::map<vao_key, GLuint>               m_vaos;
        std::map<mge::uniform_block*, GLuint>   m_ubos;
        std::map<mge::uniform_block*, uint64_t> m_ubo_versions;
    };

} // namespace mge::opengl

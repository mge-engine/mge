// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context_base.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "index_buffer.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/memory_image.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {

    static inline GLenum blend_factor_to_gl(blend_factor factor)
    {
        switch (factor) {
        case blend_factor::ZERO:
            return GL_ZERO;
        case blend_factor::ONE:
            return GL_ONE;
        case blend_factor::SRC_COLOR:
            return GL_SRC_COLOR;
        case blend_factor::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case blend_factor::DST_COLOR:
            return GL_DST_COLOR;
        case blend_factor::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case blend_factor::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case blend_factor::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case blend_factor::DST_ALPHA:
            return GL_DST_ALPHA;
        case blend_factor::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case blend_factor::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case blend_factor::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case blend_factor::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case blend_factor::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case blend_factor::SRC_ALPHA_SATURATE:
            return GL_SRC_ALPHA_SATURATE;
        case blend_factor::SRC1_COLOR:
            return GL_SRC1_COLOR;
        case blend_factor::ONE_MINUS_SRC1_COLOR:
            return GL_ONE_MINUS_SRC1_COLOR;
        case blend_factor::SRC1_ALPHA:
            return GL_SRC1_ALPHA;
        case blend_factor::ONE_MINUS_SRC1_ALPHA:
            return GL_ONE_MINUS_SRC1_ALPHA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend factor: " << factor;
        }
    }

    static inline GLenum blend_operation_to_gl(blend_operation op)
    {
        switch (op) {
        case blend_operation::NONE:
            return GL_FUNC_ADD;
        case blend_operation::ADD:
            return GL_FUNC_ADD;
        case blend_operation::SUBTRACT:
            return GL_FUNC_SUBTRACT;
        case blend_operation::REVERSE_SUBTRACT:
            return GL_FUNC_REVERSE_SUBTRACT;
        case blend_operation::MIN:
            return GL_MIN;
        case blend_operation::MAX:
            return GL_MAX;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend operation: " << op;
        }
    }

    static inline GLenum depth_test_to_gl(mge::test func)
    {
        switch (func) {
        case mge::test::NEVER:
            return GL_NEVER;
        case mge::test::LESS:
            return GL_LESS;
        case mge::test::EQUAL:
            return GL_EQUAL;
        case mge::test::LESS_EQUAL:
            return GL_LEQUAL;
        case mge::test::GREATER:
            return GL_GREATER;
        case mge::test::NOT_EQUAL:
            return GL_NOTEQUAL;
        case mge::test::GREATER_EQUAL:
            return GL_GEQUAL;
        case mge::test::ALWAYS:
            return GL_ALWAYS;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown depth test: " << static_cast<int>(func);
        }
    }

    bool                   render_context_base::s_gl3w_initialized = false;
    singleton<opengl_info> render_context_base::s_glinfo;

    render_context_base::render_context_base(
        mge::opengl::render_system& render_system_,
        const mge::extent&          ext)
        : mge::render_context(render_system_, ext)
        , m_render_system(render_system_)
    {}

    render_context_base::~render_context_base() {}

    void render_context_base::init_gl3w()
    {
        if (!s_gl3w_initialized) {
            MGE_INFO_TRACE(OPENGL, "Initializing gl3w");
            auto rc = gl3wInit();
            if (rc) {
                MGE_THROW(runtime_exception)
                    << MGE_CALLED_FUNCTION(gl3wInit)
                    << "Initializing gl3w library failed: " << rc;
            }
            s_gl3w_initialized = true;
        }
    }

    void render_context_base::collect_opengl_info()
    {
        MGE_INFO_TRACE(OPENGL, "Collecting OpenGL information");
        s_glinfo.ptr();
    }

    void render_context_base::init_capabilities()
    {
        class capabilities : public mge::render_context::capabilities
        {
        public:
            capabilities()
            {
                GLint v = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &v);
                m_max_texture_size = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &v);
                m_max_texture_3d_size = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &v);
                m_max_texture_cube_size = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &v);
                m_max_texture_array_layers = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v);
                m_max_vertex_attributes = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &v);
                m_max_uniform_buffer_bindings = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &v);
                m_max_texture_bindings = static_cast<uint32_t>(v);
                glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &v);
                m_max_color_attachments = static_cast<uint32_t>(v);
            }
            ~capabilities() = default;

            uint32_t max_texture_size() const override
            {
                return m_max_texture_size;
            }
            uint32_t max_texture_3d_size() const override
            {
                return m_max_texture_3d_size;
            }
            uint32_t max_texture_cube_size() const override
            {
                return m_max_texture_cube_size;
            }
            uint32_t max_texture_array_layers() const override
            {
                return m_max_texture_array_layers;
            }
            uint32_t max_vertex_attributes() const override
            {
                return m_max_vertex_attributes;
            }
            uint32_t max_uniform_buffer_bindings() const override
            {
                return m_max_uniform_buffer_bindings;
            }
            uint32_t max_texture_bindings() const override
            {
                return m_max_texture_bindings;
            }
            uint32_t max_color_attachments() const override
            {
                return m_max_color_attachments;
            }

        private:
            uint32_t m_max_texture_size{0};
            uint32_t m_max_texture_3d_size{0};
            uint32_t m_max_texture_cube_size{0};
            uint32_t m_max_texture_array_layers{0};
            uint32_t m_max_vertex_attributes{0};
            uint32_t m_max_uniform_buffer_bindings{0};
            uint32_t m_max_texture_bindings{0};
            uint32_t m_max_color_attachments{0};
        };

        m_capabilities = std::make_unique<capabilities>();
    }

    mge::index_buffer*
    render_context_base::on_create_index_buffer(mge::data_type dt,
                                                size_t         data_size)
    {
        return new index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer* render_context_base::on_create_vertex_buffer(
        const mge::vertex_layout& layout,
        size_t                    data_size)
    {
        return new mge::opengl::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context_base::on_create_shader(mge::shader_type t)
    {
        return new shader(*this, t);
    }

    mge::program* render_context_base::on_create_program()
    {
        return new mge::opengl::program(*this);
    }

    mge::frame_buffer* render_context_base::on_create_frame_buffer(
        const mge::frame_buffer_info& info)
    {
        auto fb = std::make_unique<opengl::frame_buffer>(*this);

        const auto num_color =
            mge::checked_cast<uint32_t>(info.color_attachments.size());
        for (uint32_t i = 0; i < num_color; ++i) {
            const auto& ca  = info.color_attachments[i];
            auto        tex = create_render_target_texture(
                mge::texture_type::TYPE_2D, ca.format, ca.extent);
            fb->attach_color(tex, i);
        }

        if (info.depth_stencil_extent) {
            mge::image_format depth_fmt(
                mge::image_format::data_format::DEPTH_STENCIL,
                mge::data_type::UINT32);
            auto tex =
                create_render_target_texture(mge::texture_type::TYPE_2D,
                                             depth_fmt,
                                             *info.depth_stencil_extent);
            fb->attach_depth(tex);
        }

        return fb.release();
    }

    mge::texture_ref render_context_base::create_texture(mge::texture_type type)
    {
        return std::make_shared<texture>(*this, type);
    }

    mge::texture_ref render_context_base::create_render_target_texture(
        mge::texture_type        type,
        const mge::image_format& format,
        const mge::extent&       extent)
    {
        return std::make_shared<texture>(*this,
                                         type,
                                         format,
                                         extent,
                                         mge::texture_usage::RENDER_TARGET);
    }

    mge::rectangle render_context_base::default_scissor() const
    {
        return mge::rectangle(0, 0, m_extent.width, m_extent.height);
    }

    uint32_t render_context_base::window_height() const
    {
        return m_extent.height;
    }

    void render_context_base::draw_geometry(
        mge::program*                    program,
        mge::vertex_buffer*              vb,
        mge::index_buffer*               ib,
        mge::uniform_block*              ub,
        const mge::texture_binding_list& textures,
        uint32_t                         index_count,
        uint32_t                         index_offset)
    {
        if (!program) {
            MGE_THROW(illegal_state) << "Draw command has no program assigned";
        }
        if (program->needs_link()) {
            MGE_THROW(illegal_state) << "Draw command has unlinked program "
                                     << (void*)program << " assigned";
        }

        mge::opengl::program& gl_program =
            static_cast<opengl::program&>(*program);
        glUseProgram(gl_program.program_name());
        CHECK_OPENGL_ERROR(glUseProgram);

        if (ub) {
            bind_uniform_block(gl_program, *ub);
        }

        for (const auto& binding : textures) {
            if (binding.texture) {
                mge::opengl::texture& gl_tex =
                    static_cast<opengl::texture&>(*binding.texture);
                glActiveTexture(GL_TEXTURE0 + binding.slot);
                CHECK_OPENGL_ERROR(glActiveTexture);
                glBindTexture(GL_TEXTURE_2D, gl_tex.texture_name());
                CHECK_OPENGL_ERROR(glBindTexture);
            }
        }
        for (const auto& sampler : gl_program.sampler_bindings()) {
            glUniform1i(static_cast<GLint>(sampler.binding),
                        static_cast<GLint>(sampler.binding));
            CHECK_OPENGL_ERROR(glUniform1i);
        }

        if (!vb) {
            MGE_THROW(illegal_state)
                << "Draw command has invalid vertex buffer";
        }
        if (!ib) {
            MGE_THROW(illegal_state) << "Draw command has invalid index buffer";
        }

        mge::opengl::vertex_buffer& gl_vb =
            static_cast<opengl::vertex_buffer&>(*vb);
        mge::opengl::index_buffer& gl_ib =
            static_cast<opengl::index_buffer&>(*ib);

        vao_key key = std::make_tuple(gl_vb.buffer_name(), gl_ib.buffer_name());
        GLuint  vao = 0;
        auto    it  = m_vaos.find(key);
        if (it != m_vaos.end()) {
            vao = it->second;
        } else {
            vao = create_vao(&gl_vb, &gl_ib);
        }
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        GLsizei count = index_count > 0
                            ? static_cast<GLsizei>(index_count)
                            : static_cast<GLsizei>(gl_ib.element_count());

        GLenum index_type;
        size_t index_size;
        switch (gl_ib.element_type()) {
        case mge::data_type::UINT16:
            index_type = GL_UNSIGNED_SHORT;
            index_size = sizeof(uint16_t);
            break;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            index_type = GL_UNSIGNED_INT;
            index_size = sizeof(uint32_t);
            break;
        default:
            MGE_THROW(mge::illegal_state)
                << "Unsupported index buffer type: "
                << static_cast<int>(gl_ib.element_type());
        }

        const void* offset_ptr =
            reinterpret_cast<const void*>(index_offset * index_size);
        glDrawElements(GL_TRIANGLES, count, index_type, offset_ptr);
        CHECK_OPENGL_ERROR(glDrawElements);
        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));
        for (const auto& binding : textures) {
            if (binding.texture) {
                glActiveTexture(GL_TEXTURE0 + binding.slot);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        glUseProgram(0);
        CHECK_OPENGL_ERROR(glUseProgram(0));
    }

    void render_context_base::bind_uniform_block(mge::opengl::program& gl_program,
                                                 mge::uniform_block&   ub)
    {
        ub.sync_from_globals();

        GLuint block_index = gl_program.block_index(ub.name());
        if (block_index == GL_INVALID_INDEX) {
            MGE_THROW(mge::no_such_element)
                << "Program has no uniform block named '" << ub.name() << "'";
        }

        GLuint ubo = 0;
        auto   it  = m_ubos.find(&ub);
        if (it != m_ubos.end()) {
            ubo = it->second;
        } else {
            glGenBuffers(1, &ubo);
            CHECK_OPENGL_ERROR(glGenBuffers);
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            CHECK_OPENGL_ERROR(glBindBuffer);
            glBufferData(GL_UNIFORM_BUFFER,
                         static_cast<GLsizeiptr>(ub.data_size()),
                         nullptr,
                         GL_DYNAMIC_DRAW);
            CHECK_OPENGL_ERROR(glBufferData);
            m_ubos[&ub]         = ubo;
            m_ubo_versions[&ub] = 0;
        }

        auto& cached_version = m_ubo_versions[&ub];
        if (cached_version != ub.version()) {
            glBindBuffer(GL_UNIFORM_BUFFER, ubo);
            CHECK_OPENGL_ERROR(glBindBuffer);
            glBufferSubData(GL_UNIFORM_BUFFER,
                            0,
                            static_cast<GLsizeiptr>(ub.data_size()),
                            ub.data());
            CHECK_OPENGL_ERROR(glBufferSubData);
            cached_version = ub.version();
        }

        GLuint binding_point = 0;
        glUniformBlockBinding(gl_program.program_name(),
                              block_index,
                              binding_point);
        CHECK_OPENGL_ERROR(glUniformBlockBinding);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo);
        CHECK_OPENGL_ERROR(glBindBufferBase);
    }

    void render_context_base::render(const mge::pass& p)
    {
        GLuint fb = 0;
        if (p.frame_buffer()) {
            auto* fbo =
                static_cast<opengl::frame_buffer*>(p.frame_buffer().get());
            fb = fbo->fbo_name();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        CHECK_OPENGL_ERROR(glBindFramebuffer);

        const auto& vp = p.viewport();
        glViewport(static_cast<const GLint>(vp.x),
                   static_cast<const GLint>(vp.y),
                   static_cast<const GLsizei>(vp.width),
                   static_cast<const GLsizei>(vp.height));
        CHECK_OPENGL_ERROR(glViewport);

        glDepthRangef(vp.min_depth, vp.max_depth);
        CHECK_OPENGL_ERROR(glDepthRangef);

        GLint wh = fb != 0 ? static_cast<GLint>(vp.height)
                           : static_cast<GLint>(window_height());
        glScissor(static_cast<const GLint>(p.scissor().left),
                  wh - static_cast<const GLint>(p.scissor().bottom),
                  static_cast<const GLsizei>(p.scissor().width()),
                  static_cast<const GLsizei>(p.scissor().height()));
        CHECK_OPENGL_ERROR(glScissor);

        if (p.clear_color_enabled()) {
            const rgba_color& c = p.clear_color_value();
            glClearColor(c.r, c.g, c.b, c.a);
            CHECK_OPENGL_ERROR(glClearColor);
            glClear(GL_COLOR_BUFFER_BIT);
            CHECK_OPENGL_ERROR(glClear);
        }

        if (p.clear_depth_enabled()) {
            glClearDepthf(static_cast<GLfloat>(p.clear_depth_value()));
            CHECK_OPENGL_ERROR(glClearDepthf);
            glClear(GL_DEPTH_BUFFER_BIT);
            CHECK_OPENGL_ERROR(glClear);
        }

        glEnable(GL_DEPTH_TEST);
        CHECK_OPENGL_ERROR(glEnable);

        glEnable(GL_SCISSOR_TEST);
        CHECK_OPENGL_ERROR(glEnable);

#ifndef GL_CONSERVATIVE_RASTERIZATION_NV
#    define GL_CONSERVATIVE_RASTERIZATION_NV 0x9346
#endif
#ifndef GL_CONSERVATIVE_RASTERIZATION_INTEL
#    define GL_CONSERVATIVE_RASTERIZATION_INTEL 0x9346
#endif

        bool           blend_pass_needed = false;
        mge::rectangle current_scissor   = p.scissor();
        for_each_draw_in_pass(
            p.index(),
            [this, wh, &blend_pass_needed, &current_scissor, &p](
                const program_handle&            program,
                const vertex_buffer_handle&      vertices,
                const index_buffer_handle&       indices,
                const mge::pipeline_state&       state,
                mge::uniform_block*              ub,
                const mge::texture_binding_list& textures,
                uint32_t                         index_count,
                uint32_t                         index_offset,
                const mge::rectangle&            cmd_scissor) {
                blend_operation op = state.color_blend_operation();
                if (op == blend_operation::NONE) {
                    const auto& effective =
                        cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                    if (effective != current_scissor) {
                        glScissor(
                            static_cast<const GLint>(effective.left),
                            wh - static_cast<const GLint>(effective.bottom),
                            static_cast<const GLsizei>(effective.width()),
                            static_cast<const GLsizei>(effective.height()));
                        current_scissor = effective;
                    }
                    mge::cull_mode cull = state.cull_mode();
                    if (cull != mge::cull_mode::NONE) {
                        glEnable(GL_CULL_FACE);
                        CHECK_OPENGL_ERROR(glEnable);
                        if (cull == mge::cull_mode::CLOCKWISE) {
                            glCullFace(GL_FRONT);
                        } else {
                            glCullFace(GL_BACK);
                        }
                        CHECK_OPENGL_ERROR(glCullFace);
                    }
                    glDepthFunc(depth_test_to_gl(state.depth_test_function()));
                    CHECK_OPENGL_ERROR(glDepthFunc);
                    if (!state.depth_write()) {
                        glDepthMask(GL_FALSE);
                        CHECK_OPENGL_ERROR(glDepthMask);
                    }
                    bool conservative_raster_enabled =
                        m_conservative_rasterization_supported &&
                        state.test(pipeline_state::CONSERVATIVE_RASTERIZATION);
                    if (conservative_raster_enabled) {
                        glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
                        CHECK_OPENGL_ERROR(glEnable);
                    }
                    draw_geometry(program.get(),
                                  vertices.get(),
                                  indices.get(),
                                  ub,
                                  textures,
                                  index_count,
                                  index_offset);
                    if (conservative_raster_enabled) {
                        glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
                        CHECK_OPENGL_ERROR(glDisable);
                    }
                    if (!state.depth_write()) {
                        glDepthMask(GL_TRUE);
                        CHECK_OPENGL_ERROR(glDepthMask);
                    }
                    if (cull != mge::cull_mode::NONE) {
                        glDisable(GL_CULL_FACE);
                        CHECK_OPENGL_ERROR(glDisable);
                    }
                } else {
                    blend_pass_needed = true;
                }
            });

        if (blend_pass_needed) {
            glEnable(GL_BLEND);
            CHECK_OPENGL_ERROR(glEnable);
            for_each_draw_in_pass(
                p.index(),
                [this, wh, &current_scissor, &p](
                    const program_handle&            program,
                    const vertex_buffer_handle&      vertices,
                    const index_buffer_handle&       indices,
                    const mge::pipeline_state&       state,
                    mge::uniform_block*              ub,
                    const mge::texture_binding_list& textures,
                    uint32_t                         index_count,
                    uint32_t                         index_offset,
                    const mge::rectangle&            cmd_scissor) {
                    blend_operation color_op  = state.color_blend_operation();
                    blend_operation alpha_op  = state.alpha_blend_operation();
                    blend_factor    color_src = state.color_blend_factor_src();
                    blend_factor    color_dst = state.color_blend_factor_dst();
                    blend_factor    alpha_src = state.alpha_blend_factor_src();
                    blend_factor    alpha_dst = state.alpha_blend_factor_dst();
                    if (color_op != blend_operation::NONE) {
                        const auto& effective =
                            cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                        if (effective != current_scissor) {
                            glScissor(
                                static_cast<const GLint>(effective.left),
                                wh - static_cast<const GLint>(effective.bottom),
                                static_cast<const GLsizei>(effective.width()),
                                static_cast<const GLsizei>(effective.height()));
                            current_scissor = effective;
                        }
                        mge::cull_mode cull = state.cull_mode();
                        if (cull != mge::cull_mode::NONE) {
                            glEnable(GL_CULL_FACE);
                            CHECK_OPENGL_ERROR(glEnable);
                            if (cull == mge::cull_mode::CLOCKWISE) {
                                glCullFace(GL_FRONT);
                            } else {
                                glCullFace(GL_BACK);
                            }
                            CHECK_OPENGL_ERROR(glCullFace);
                        }
                        glDepthFunc(
                            depth_test_to_gl(state.depth_test_function()));
                        CHECK_OPENGL_ERROR(glDepthFunc);
                        if (color_op == alpha_op && color_src == alpha_src &&
                            color_dst == alpha_dst) {
                            glBlendFunc(blend_factor_to_gl(color_src),
                                        blend_factor_to_gl(color_dst));
                            CHECK_OPENGL_ERROR(glBlendFunc);
                            glBlendEquation(blend_operation_to_gl(color_op));
                            CHECK_OPENGL_ERROR(glBlendEquation);
                        } else {
                            glBlendFuncSeparate(blend_factor_to_gl(color_src),
                                                blend_factor_to_gl(color_dst),
                                                blend_factor_to_gl(alpha_src),
                                                blend_factor_to_gl(alpha_dst));
                            CHECK_OPENGL_ERROR(glBlendFuncSeparate);
                            glBlendEquationSeparate(
                                blend_operation_to_gl(color_op),
                                blend_operation_to_gl(alpha_op));
                            CHECK_OPENGL_ERROR(glBlendEquationSeparate);
                        }
                        if (!state.depth_write()) {
                            glDepthMask(GL_FALSE);
                            if (cull != mge::cull_mode::NONE) {
                                glDisable(GL_CULL_FACE);
                                CHECK_OPENGL_ERROR(glDisable);
                            }
                            CHECK_OPENGL_ERROR(glDepthMask);
                        }
                        bool conservative_raster_enabled =
                            m_conservative_rasterization_supported &&
                            state.test(
                                pipeline_state::CONSERVATIVE_RASTERIZATION);
                        if (conservative_raster_enabled) {
                            glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
                            CHECK_OPENGL_ERROR(glEnable);
                        }
                        draw_geometry(program.get(),
                                      vertices.get(),
                                      indices.get(),
                                      ub,
                                      textures,
                                      index_count,
                                      index_offset);
                        if (conservative_raster_enabled) {
                            glDisable(GL_CONSERVATIVE_RASTERIZATION_NV);
                            CHECK_OPENGL_ERROR(glDisable);
                        }
                        if (!state.depth_write()) {
                            glDepthMask(GL_TRUE);
                            CHECK_OPENGL_ERROR(glDepthMask);
                        }
                    }
                });
            glDisable(GL_BLEND);
            CHECK_OPENGL_ERROR(glDisable);
        }

        glDisable(GL_SCISSOR_TEST);
        CHECK_OPENGL_ERROR(glDisable);

        glDisable(GL_DEPTH_TEST);
        CHECK_OPENGL_ERROR(glDisable);
    }

    GLuint render_context_base::create_vao(mge::opengl::vertex_buffer* vb,
                                           mge::opengl::index_buffer*  ib)
    {
        vao_key key = std::make_tuple(vb->buffer_name(), ib->buffer_name());
        GLuint  vao = 0;
        glGenVertexArrays(1, &vao);
        CHECK_OPENGL_ERROR(glGenVertexArrays);
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vb->buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto& layout = vb->layout();
        GLsizei     stride = static_cast<GLsizei>(layout.stride());
        uint32_t    index  = 0;
        for (const auto& f : layout.formats()) {
            glEnableVertexAttribArray(index);
            CHECK_OPENGL_ERROR(glEnableVertexAttribArray);
            auto offset = reinterpret_cast<const void*>(layout.offset(index));
            switch (f.type()) {
            case mge::data_type::FLOAT:
                glVertexAttribPointer(index,
                                      f.size(),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      stride,
                                      offset);
                CHECK_OPENGL_ERROR(glVertexAttribPointer);
                break;
            case mge::data_type::UINT8:
                glVertexAttribPointer(index,
                                      f.size(),
                                      GL_UNSIGNED_BYTE,
                                      GL_TRUE,
                                      stride,
                                      offset);
                CHECK_OPENGL_ERROR(glVertexAttribPointer);
                break;
            default:
                MGE_THROW(opengl::error)
                    << "Unsupported vertex array element type " << f.type();
            }
            ++index;
        }
        glBindVertexArray(0);
        CHECK_OPENGL_ERROR(glBindVertexArray(0));

        return m_vaos[key] = vao;
    }

} // namespace mge::opengl

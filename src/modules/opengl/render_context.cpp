// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/parameter.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

#ifdef MGE_OS_WINDOWS
// WGL extension constants and types
#    define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#    define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#    define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#    define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC   hDC,
                                                         HGLRC hShareContext,
                                                         const int* attribList);
#endif

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
            return GL_FUNC_ADD; // Default, though NONE shouldn't reach here
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

#ifdef MGE_OS_WINDOWS
    render_context::render_context(mge::opengl::render_system& render_system_,
                                   mge::opengl::window*        context_window)
        : mge::render_context(render_system_, context_window->extent())
        , m_window(context_window)
        , m_hwnd(context_window->hwnd())
        , m_hdc(0)
        , m_primary_hglrc(0)
        , m_lock("opengl::render_context")
    {
        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(GetDC);
        }
        select_pixel_format();
        create_primary_glrc();
        init_gl3w();
        collect_opengl_info();

        // Check for conservative rasterization support
        const auto& exts = gl_info().extensions;
        m_conservative_rasterization_supported =
            exts.find("GL_NV_conservative_raster") != exts.end() ||
            exts.find("GL_INTEL_conservative_rasterization") != exts.end();
        if (m_conservative_rasterization_supported) {
            MGE_INFO_TRACE(OPENGL, "Conservative rasterization supported");
        }

        // Set clip control to keep OpenGL Y orientation while using 0..1 depth
        // - GL_LOWER_LEFT: keep OpenGL viewport origin
        // - GL_ZERO_TO_ONE: depth range [0,1] (matches DirectX/Vulkan)
        if (gl_info().major_version > 4 ||
            (gl_info().major_version == 4 && gl_info().minor_version >= 5)) {
            MGE_INFO_TRACE(
                OPENGL,
                "Setting clip control: GL_LOWER_LEFT, GL_ZERO_TO_ONE");
            glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
            CHECK_OPENGL_ERROR(glClipControl);
        } else {
            MGE_WARNING_TRACE(OPENGL,
                              "OpenGL {}.{} does not support glClipControl "
                              "(requires 4.5+)",
                              gl_info().major_version,
                              gl_info().minor_version);
        }

        auto fd = render_system_.frame_debugger();
        if (fd) {
            fd->set_context(
                frame_debugger::capture_context{m_primary_hglrc, nullptr});
        }
    }

    void render_context::select_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        mge::zero_memory(pixel_format_desc);

        pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits = 32;
        pixel_format_desc.cAlphaBits = 8;
        pixel_format_desc.cDepthBits = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(SetPixelFormat);
        }
    }

    void render_context::create_primary_glrc()
    {
        // Create a temporary legacy context to load WGL extensions
        HGLRC temp_context = wglCreateContext(m_hdc);
        if (!temp_context) {
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, temp_context)) {
            wglDeleteContext(temp_context);
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }

        // Load wglCreateContextAttribsARB
#    pragma warning(push)
#    pragma warning(disable : 4191) // unsafe conversion from PROC
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress(
                "wglCreateContextAttribsARB");
#    pragma warning(pop)

        HGLRC hglrc = nullptr;
        if (wglCreateContextAttribsARB) {
            // Create a modern OpenGL 4.6 context
            const int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                                   4,
                                   WGL_CONTEXT_MINOR_VERSION_ARB,
                                   6,
                                   WGL_CONTEXT_PROFILE_MASK_ARB,
                                   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                   0};
            hglrc = wglCreateContextAttribsARB(m_hdc, 0, attribs);
            if (!hglrc) {
                MGE_THROW(system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContextAttribsARB);
            }

        } else {
            MGE_DEBUG_TRACE(OPENGL,
                            "wglCreateContextAttribsARB not available, "
                            "falling back to legacy context");
            // Fallback: Create a legacy context
            hglrc = wglCreateContext(m_hdc);
            if (!hglrc) {
                MGE_THROW(system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContext);
            }
        }

        // Delete temporary context
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temp_context);

        if (!wglMakeCurrent(m_hdc, hglrc)) {
            wglDeleteContext(hglrc);
            MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
        {
            std::scoped_lock<mge::mutex> scope(m_lock);
            m_thread_glrcs[mge::this_thread::get_id()] = hglrc;
        }
        m_primary_hglrc = hglrc;
    }

    static bool s_gl3w_initialized = false;

    void render_context::init_gl3w()
    {
        if (!s_gl3w_initialized) {
            MGE_INFO_TRACE(OPENGL, "Initializing gl3w");
            auto rc = gl3wInit();
            if (rc) {
                MGE_THROW(runtime_exception)
                    << MGE_CALLED_FUNCTION(gl3wInit)
                    << "Initializing gl32 library failed: " << rc;
            }
            s_gl3w_initialized = true;
        }
    }

    void render_context::collect_opengl_info()
    {
        MGE_INFO_TRACE(OPENGL, "Collecting OpenGL information");
        s_glinfo.ptr();
    }
#else
#    error Missing port
#endif

    render_context::~render_context()
    {
        if (m_render_system.frame_debugger()) {
            auto fd = m_render_system.frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(OPENGL, "Ending frame recording");
                fd->end_capture();
            }
        }
    }

    singleton<opengl_info> render_context::s_glinfo;

    mge::index_buffer* render_context::on_create_index_buffer(mge::data_type dt,
                                                              size_t data_size)
    {
        return new index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer*
    render_context::on_create_vertex_buffer(const mge::vertex_layout& layout,
                                            size_t                    data_size)
    {
        return new mge::opengl::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context::on_create_shader(mge::shader_type t)
    {
        return new shader(*this, t);
    }

    mge::program* render_context::on_create_program()
    {
        return new mge::opengl::program(*this);
    }

    mge::texture_ref render_context::create_texture(mge::texture_type type)
    {
        mge::texture_ref result = std::make_shared<texture>(*this, type);
        return result;
    }

    mge::rectangle render_context::default_scissor() const
    {
        return mge::rectangle(m_window->position(), m_window->extent());
    }

    uint32_t render_context::window_height() const
    {
        return m_window->extent().height;
    }

    void render_context::draw_geometry(mge::program*       program,
                                       mge::vertex_buffer* vb,
                                       mge::index_buffer*  ib,
                                       mge::uniform_block* ub,
                                       mge::texture*       tex,
                                       uint32_t            index_count,
                                       uint32_t            index_offset)
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

        // Bind uniform block if provided
        if (ub) {
            bind_uniform_block(gl_program, *ub);
        }

        // Bind texture if provided
        if (tex) {
            mge::opengl::texture& gl_tex = static_cast<opengl::texture&>(*tex);
            glActiveTexture(GL_TEXTURE0);
            CHECK_OPENGL_ERROR(glActiveTexture);
            glBindTexture(GL_TEXTURE_2D, gl_tex.texture_name());
            CHECK_OPENGL_ERROR(glBindTexture);
            // Set all sampler uniforms to texture unit 0
            for (const auto& sampler : gl_program.sampler_locations()) {
                glUniform1i(sampler.location, 0);
                CHECK_OPENGL_ERROR(glUniform1i);
            }
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
        auto    it = m_vaos.find(key);
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
        if (tex) {
            glBindTexture(GL_TEXTURE_2D, 0);
            CHECK_OPENGL_ERROR(glBindTexture(0));
        }
        glUseProgram(0);
        CHECK_OPENGL_ERROR(glUseProgram(0));
    }

    void render_context::bind_uniform_block(mge::opengl::program& gl_program,
                                            mge::uniform_block&   ub)
    {
        // Sync values from global uniforms
        ub.sync_from_globals();

        // Get cached block index from the GL program
        GLuint block_index = gl_program.block_index(ub.name());
        if (block_index == GL_INVALID_INDEX) {
            MGE_THROW(mge::no_such_element)
                << "Program has no uniform block named '" << ub.name() << "'";
        }

        // Get or create the GL buffer object for this block
        GLuint ubo = 0;
        auto   it = m_ubos.find(&ub);
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
            m_ubos[&ub] = ubo;
            m_ubo_versions[&ub] = 0;
        }

        // Upload data if the block version changed
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

        // Bind the UBO to binding point 0 and connect the block to it
        GLuint binding_point = 0;
        glUniformBlockBinding(gl_program.program_name(),
                              block_index,
                              binding_point);
        CHECK_OPENGL_ERROR(glUniformBlockBinding);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo);
        CHECK_OPENGL_ERROR(glBindBufferBase);
    }

    void render_context::render(const mge::pass& p)
    {
        GLuint fb = 0;
        if (p.frame_buffer()) {
            MGE_THROW_NOT_IMPLEMENTED
                << "Rendering to custom frame buffers not implemented";
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

        glScissor(static_cast<const GLint>(p.scissor().left),
                  static_cast<const GLint>(p.scissor().top),
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

#ifndef GL_CONSERVATIVE_RASTERIZATION_NV
#    define GL_CONSERVATIVE_RASTERIZATION_NV 0x9346
#endif
#ifndef GL_CONSERVATIVE_RASTERIZATION_INTEL
#    define GL_CONSERVATIVE_RASTERIZATION_INTEL 0x9346
#endif

        bool           blend_pass_needed = false;
        mge::rectangle current_scissor = p.scissor();
        p.for_each_draw_command([this,
                                 &blend_pass_needed,
                                 &current_scissor,
                                 &p](const program_handle&       program,
                                     const vertex_buffer_handle& vertices,
                                     const index_buffer_handle&  indices,
                                     const mge::pipeline_state&  state,
                                     mge::uniform_block*         ub,
                                     mge::texture*               tex,
                                     uint32_t                    index_count,
                                     uint32_t                    index_offset,
                                     const mge::rectangle&       cmd_scissor) {
            blend_operation op = state.color_blend_operation();
            if (op == blend_operation::NONE) {
                const auto& effective =
                    cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                if (effective != current_scissor) {
                    glScissor(static_cast<const GLint>(effective.left),
                              static_cast<const GLint>(effective.top),
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
                              tex,
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
            p.for_each_draw_command([this, &current_scissor, &p](
                                        const program_handle&       program,
                                        const vertex_buffer_handle& vertices,
                                        const index_buffer_handle&  indices,
                                        const mge::pipeline_state&  state,
                                        mge::uniform_block*         ub,
                                        mge::texture*               tex,
                                        uint32_t                    index_count,
                                        uint32_t              index_offset,
                                        const mge::rectangle& cmd_scissor) {
                blend_operation color_op = state.color_blend_operation();
                blend_operation alpha_op = state.alpha_blend_operation();
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
                            static_cast<const GLint>(effective.top),
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
                        state.test(pipeline_state::CONSERVATIVE_RASTERIZATION);
                    if (conservative_raster_enabled) {
                        glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
                        CHECK_OPENGL_ERROR(glEnable);
                    }
                    draw_geometry(program.get(),
                                  vertices.get(),
                                  indices.get(),
                                  ub,
                                  tex,
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

        glDisable(GL_DEPTH_TEST);
        CHECK_OPENGL_ERROR(glDisable);
    }

    mge::image_ref render_context::screenshot()
    {
        return image_ref();
    }

    void render_context::on_frame_present()
    {
#ifdef MGE_OS_WINDOWS
        SwapBuffers(m_hdc);
#else
#    error Missing port
#endif
    }

    GLuint render_context::create_vao(mge::opengl::vertex_buffer* vb,
                                      mge::opengl::index_buffer*  ib)
    {
        vao_key key = std::make_tuple(vb->buffer_name(), ib->buffer_name());
        GLuint  vao = 0;
        glCreateVertexArrays(1, &vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vb->buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto& layout = vb->layout();
        GLsizei     stride = static_cast<GLsizei>(layout.stride());
        uint32_t    index = 0;
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
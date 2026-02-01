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
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
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

        if (p.clear_color_enabled()) {
            const rgba_color& c = p.clear_color_value();
            glClearColor(c.r, c.g, c.b, c.a);
            CHECK_OPENGL_ERROR(glClearColor);
            glClear(GL_COLOR_BUFFER_BIT);
            CHECK_OPENGL_ERROR(glClear);
        }

        p.for_each_draw_command([this](const program_handle& program_handle,
                                       const vertex_buffer_handle& vertices,
                                       const index_buffer_handle&  indices) {
            auto program = program_handle.get();
            if (!program) {
                MGE_THROW(illegal_state)
                    << "Draw command has no program assigned";
            }
            if (program->needs_link()) {
                MGE_THROW(illegal_state) << "Draw command has unlinked program "
                                         << (void*)program << " assigned";
            }
            mge::opengl::program& gl_program =
                static_cast<opengl::program&>(*program);
            glUseProgram(gl_program.program_name());
            CHECK_OPENGL_ERROR(glUseProgram);
            auto vb = vertices.get();
            if (!vb) {
                MGE_THROW(illegal_state)
                    << "Draw command has invalid vertex buffer";
            }
            auto ib = indices.get();
            if (!ib) {
                MGE_THROW(illegal_state)
                    << "Draw command has invalid index buffer";
            }

            mge::opengl::vertex_buffer& gl_vb =
                static_cast<opengl::vertex_buffer&>(*vb);
            mge::opengl::index_buffer& gl_ib =
                static_cast<opengl::index_buffer&>(*ib);

            vao_key key =
                std::make_tuple(gl_vb.buffer_name(), gl_ib.buffer_name());
            GLuint vao = 0;
            auto   it = m_vaos.find(key);
            if (it != m_vaos.end()) {
                vao = it->second;
            } else {
                vao = create_vao(gl_vb, gl_ib);
            }
            glBindVertexArray(vao);
            CHECK_OPENGL_ERROR(glBindVertexArray);
            glDrawElements(GL_TRIANGLES,
                           static_cast<GLsizei>(gl_ib.element_count()),
                           GL_UNSIGNED_INT,
                           nullptr);
            CHECK_OPENGL_ERROR(glDrawElements);
            glBindVertexArray(0);
            CHECK_OPENGL_ERROR(glBindVertexArray(0));
            glUseProgram(0);
            CHECK_OPENGL_ERROR(glUseProgram(0));
        });
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

    GLuint render_context::create_vao(mge::opengl::vertex_buffer& vb,
                                      mge::opengl::index_buffer&  ib)
    {
        vao_key key = std::make_tuple(vb.buffer_name(), ib.buffer_name());
        GLuint  vao = 0;
        glCreateVertexArrays(1, &vao);
        CHECK_OPENGL_ERROR(glCreateVertexArrays);
        glBindVertexArray(vao);
        CHECK_OPENGL_ERROR(glBindVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vb.buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ARRAY_BUFFER));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.buffer_name());
        CHECK_OPENGL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER));

        const auto& layout = vb.layout();
        uint32_t    index = 0;
        for (const auto& f : layout.formats()) {
            glEnableVertexAttribArray(index);
            CHECK_OPENGL_ERROR(glEnableVertexAttribArray);
            switch (f.type()) {
            case mge::data_type::FLOAT:
                glVertexAttribPointer(index,
                                      f.size(),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0,
                                      nullptr);
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
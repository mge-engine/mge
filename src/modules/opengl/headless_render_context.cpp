// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "headless_render_context.hpp"
#include "error.hpp"
#include "frame_buffer.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_system.hpp"

#ifdef MGE_OS_WINDOWS
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

    headless_render_context::headless_render_context(
        mge::opengl::render_system& render_system_,
        const mge::extent&          extent)
        : render_context_base(render_system_, extent)
    {
        MGE_DEBUG_TRACE(OPENGL, "Create headless OpenGL render context");
#ifdef MGE_OS_WINDOWS
        create_hidden_window();
        select_pixel_format();
        create_glrc();
#endif
        init_gl3w();
        collect_opengl_info();
        init_capabilities();

        const auto& exts = gl_info().extensions;
        m_conservative_rasterization_supported =
            exts.find("GL_NV_conservative_raster") != exts.end() ||
            exts.find("GL_INTEL_conservative_rasterization") != exts.end();

        if (gl_info().major_version > 4 ||
            (gl_info().major_version == 4 && gl_info().minor_version >= 5)) {
            glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
            CHECK_OPENGL_ERROR(glClipControl);
        }
    }

    headless_render_context::~headless_render_context()
    {
#ifdef MGE_OS_WINDOWS
        if (m_hglrc) {
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(m_hglrc);
            m_hglrc = nullptr;
        }
        if (m_hdc && m_hwnd) {
            ReleaseDC(m_hwnd, m_hdc);
            m_hdc = nullptr;
        }
        if (m_hwnd) {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
#endif
    }

#ifdef MGE_OS_WINDOWS
    void headless_render_context::create_hidden_window()
    {
        static ATOM wc_atom = 0;
        if (!wc_atom) {
            WNDCLASSEXA wc   = {};
            wc.cbSize        = sizeof(wc);
            wc.style         = CS_OWNDC;
            wc.lpfnWndProc   = DefWindowProcA;
            wc.hInstance     = GetModuleHandleA(nullptr);
            wc.lpszClassName = "mge_opengl_headless";
            wc_atom          = RegisterClassExA(&wc);
            if (!wc_atom) {
                MGE_CHECK_SYSTEM_ERROR(RegisterClassExA);
            }
        }

        m_hwnd = CreateWindowExA(0,
                                 "mge_opengl_headless",
                                 "",
                                 WS_POPUP,
                                 0,
                                 0,
                                 static_cast<int>(m_extent.width),
                                 static_cast<int>(m_extent.height),
                                 nullptr,
                                 nullptr,
                                 GetModuleHandleA(nullptr),
                                 nullptr);
        if (!m_hwnd) {
            MGE_CHECK_SYSTEM_ERROR(CreateWindowExA);
        }

        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(GetDC);
        }
    }

    void headless_render_context::select_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pfd;
        mge::zero_memory(pfd);
        pfd.nSize        = sizeof(pfd);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;
        pfd.cAlphaBits   = 8;
        pfd.cDepthBits   = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType   = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pfd);
        if (pixel_format == 0) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pfd)) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(SetPixelFormat);
        }
    }

    void headless_render_context::create_glrc()
    {
        HGLRC temp_context = wglCreateContext(m_hdc);
        if (!temp_context) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, temp_context)) {
            wglDeleteContext(temp_context);
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }

#    pragma warning(push)
#    pragma warning(disable : 4191)
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
            (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress(
                "wglCreateContextAttribsARB");
#    pragma warning(pop)

        HGLRC hglrc = nullptr;
        if (wglCreateContextAttribsARB) {
            const int attribs[] = {WGL_CONTEXT_MAJOR_VERSION_ARB,
                                   4,
                                   WGL_CONTEXT_MINOR_VERSION_ARB,
                                   6,
                                   WGL_CONTEXT_PROFILE_MASK_ARB,
                                   WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                                   0};
            hglrc = wglCreateContextAttribsARB(m_hdc, 0, attribs);
            if (!hglrc) {
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(temp_context);
                MGE_THROW(mge::system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContextAttribsARB);
            }
        } else {
            hglrc = wglCreateContext(m_hdc);
            if (!hglrc) {
                wglMakeCurrent(nullptr, nullptr);
                wglDeleteContext(temp_context);
                MGE_THROW(mge::system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContext);
            }
        }

        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_context);

        if (!wglMakeCurrent(m_hdc, hglrc)) {
            wglDeleteContext(hglrc);
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
        m_hglrc = hglrc;
    }
#endif

    void headless_render_context::on_frame_present()
    {
        glFlush();
    }

    mge::image_ref headless_render_context::screenshot()
    {
        auto& p   = pass(0);
        auto* fbo = static_cast<opengl::frame_buffer*>(p.frame_buffer().get());
        if (!fbo) {
            MGE_THROW(mge::illegal_state)
                << "Headless screenshot: pass 0 has no frame buffer";
        }

        uint32_t w = m_extent.width;
        uint32_t h = m_extent.height;

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        auto img =
            std::make_shared<mge::memory_image>(fmt, mge::extent(w, h));

        glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo_name());
        CHECK_OPENGL_ERROR(glBindFramebuffer);

        glReadBuffer(GL_COLOR_ATTACHMENT0);
        CHECK_OPENGL_ERROR(glReadBuffer);

        glReadPixels(0,
                     0,
                     static_cast<GLsizei>(w),
                     static_cast<GLsizei>(h),
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     img->data());
        CHECK_OPENGL_ERROR(glReadPixels);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_OPENGL_ERROR(glBindFramebuffer);

        // Flip vertically — OpenGL origin is bottom-left
        uint32_t             row_size = w * 4;
        auto*                data     = static_cast<uint8_t*>(img->data());
        std::vector<uint8_t> row(row_size);
        for (uint32_t y = 0; y < h / 2; ++y) {
            uint8_t* top = data + y * row_size;
            uint8_t* bot = data + (h - 1 - y) * row_size;
            memcpy(row.data(), top, row_size);
            memcpy(top, bot, row_size);
            memcpy(bot, row.data(), row_size);
        }

        return img;
    }

} // namespace mge::opengl

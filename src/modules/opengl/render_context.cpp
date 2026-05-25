// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/graphics/frame_debugger.hpp"
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
#else
#    define GLFW_INCLUDE_NONE
#    include <GLFW/glfw3.h>
#endif

namespace mge {
    MGE_USE_TRACE(OPENGL);
}

namespace mge::opengl {

#ifdef MGE_OS_WINDOWS
    render_context::render_context(mge::opengl::render_system& render_system_,
                                   mge::opengl::window*        context_window)
        : render_context_base(render_system_, context_window->extent())
        , m_window(context_window)
        , m_hwnd(context_window->hwnd())
        , m_hdc(0)
        , m_primary_hglrc(0)
        , m_lock("opengl::render_context")
    {
        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(GetDC);
        }
        select_pixel_format();
        create_primary_glrc();
        init_gl3w();
        collect_opengl_info();
        init_capabilities();

        const auto& exts = gl_info().extensions;
        m_conservative_rasterization_supported =
            exts.find("GL_NV_conservative_raster") != exts.end() ||
            exts.find("GL_INTEL_conservative_rasterization") != exts.end();
        if (m_conservative_rasterization_supported) {
            MGE_INFO_TRACE(OPENGL, "Conservative rasterization supported");
        }

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

        pixel_format_desc.nSize    = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags =
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType   = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits   = 32;
        pixel_format_desc.cAlphaBits   = 8;
        pixel_format_desc.cDepthBits   = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType   = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW(mge::system_error)
                << MGE_CALLED_FUNCTION(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(SetPixelFormat);
        }
    }

    void render_context::create_primary_glrc()
    {
        HGLRC temp_context = wglCreateContext(m_hdc);
        if (!temp_context) {
            MGE_THROW(mge::system_error)
                << MGE_CALLED_FUNCTION(wglCreateContext);
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
                MGE_THROW(mge::system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContextAttribsARB);
            }
        } else {
            MGE_DEBUG_TRACE(OPENGL,
                            "wglCreateContextAttribsARB not available, "
                            "falling back to legacy context");
            hglrc = wglCreateContext(m_hdc);
            if (!hglrc) {
                MGE_THROW(mge::system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContext);
            }
        }

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temp_context);

        if (!wglMakeCurrent(m_hdc, hglrc)) {
            wglDeleteContext(hglrc);
            MGE_THROW(mge::system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
        {
            std::scoped_lock<mge::mutex> scope(m_lock);
            m_thread_glrcs[mge::this_thread::get_id()] = hglrc;
        }
        m_primary_hglrc = hglrc;
    }

#else
    render_context::render_context(mge::opengl::render_system& render_system_,
                                   mge::opengl::window*        context_window)
        : render_context_base(render_system_, context_window->extent())
        , m_window(context_window)
        , m_glfw_window(context_window->handle())
    {
        glfwMakeContextCurrent(m_glfw_window);

        int fb_width  = 0;
        int fb_height = 0;
        glfwGetFramebufferSize(m_glfw_window, &fb_width, &fb_height);
        m_extent = mge::extent(static_cast<uint32_t>(fb_width),
                               static_cast<uint32_t>(fb_height));

        init_gl3w();
        collect_opengl_info();
        init_capabilities();

        const auto& exts = gl_info().extensions;
        m_conservative_rasterization_supported =
            exts.find("GL_NV_conservative_raster") != exts.end() ||
            exts.find("GL_INTEL_conservative_rasterization") != exts.end();
        if (m_conservative_rasterization_supported) {
            MGE_INFO_TRACE(OPENGL, "Conservative rasterization supported");
        }

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
    }
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

    void render_context::on_frame_present()
    {
#ifdef MGE_OS_WINDOWS
        SwapBuffers(m_hdc);
#else
        glfwSwapBuffers(m_glfw_window);
#endif
    }

    mge::image_ref render_context::screenshot()
    {
        uint32_t w = m_extent.width;
        uint32_t h = m_extent.height;

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        auto img = std::make_shared<mge::memory_image>(fmt, mge::extent(w, h));

        glReadBuffer(GL_BACK);
        CHECK_OPENGL_ERROR(glReadBuffer);
        glReadPixels(0,
                     0,
                     static_cast<GLsizei>(w),
                     static_cast<GLsizei>(h),
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     img->data());
        CHECK_OPENGL_ERROR(glReadPixels);

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

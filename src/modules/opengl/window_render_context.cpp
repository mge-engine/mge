#include "window_render_context.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/zero_memory.hpp"
#include "mge/core/log.hpp"
#include <boost/algorithm/string.hpp>

MGE_USE_LOG(OPENGL);

namespace opengl {
#ifdef MGE_OS_WINDOWS
    window_render_context::window_render_context(window *w)
        :mge::render_context(w)
        ,m_hwnd(w->hwnd())
        ,m_hdc(nullptr)
        ,m_hglrc(nullptr)
    {
        m_hdc = GetDC(m_hwnd);
        if (!m_hdc) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(GetDC);
        }

        choose_pixel_format();
        create_glrc();
        init_gl3w();
        collect_opengl_info();
    }

    window_render_context::~window_render_context()
    {
        if(m_hdc && m_hwnd) {
            ReleaseDC(m_hwnd, m_hdc);
        }
    }

    void
    window_render_context::choose_pixel_format()
    {
        PIXELFORMATDESCRIPTOR pixel_format_desc;
        mge::zero_memory(pixel_format_desc);
        pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format_desc.nVersion = 1;
        pixel_format_desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
        pixel_format_desc.cColorBits = 32;
        pixel_format_desc.cAlphaBits = 8;
        pixel_format_desc.cDepthBits = 24;
        pixel_format_desc.cStencilBits = 8;
        pixel_format_desc.iLayerType = PFD_MAIN_PLANE;

        int pixel_format = ChoosePixelFormat(m_hdc, &pixel_format_desc);
        if (pixel_format == 0) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(ChoosePixelFormat);
        }
        if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(SetPixelFormat);
        }
    }

    void
    window_render_context::create_glrc()
    {
        m_hglrc = wglCreateContext(m_hdc);
        if (!m_hglrc) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(wglCreateContext);
        }
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
    }

    static bool s_gl3w_initialized = false;

    void
    window_render_context::init_gl3w()
    {
        if (!s_gl3w_initialized) {
            MGE_INFO_LOG(OPENGL) << "Initialize gl3w";
            auto rc = gl3wInit();
            if (rc) {
                MGE_THROW(mge::exception(),
                          "Initialization of gl3w library failed with rc ",rc);
            }
            s_gl3w_initialized = true;
        }
    }

    void
    window_render_context::collect_opengl_info()
    {
        int major_version = 0;
        int minor_version = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &major_version);
        glGetIntegerv(GL_MINOR_VERSION, &minor_version);
        MGE_INFO_LOG(OPENGL) << "OpenGL version: " << major_version << "." << minor_version;
        std::string glsl_version_str((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
        MGE_INFO_LOG(OPENGL) << "Shading language version: " << glsl_version_str;
        std::string extension_string((const char *)glGetString(GL_EXTENSIONS));
        auto it = boost::make_split_iterator(extension_string, boost::token_finder(boost::is_space()));
        decltype(it) end_it;
        MGE_INFO_LOG(OPENGL) << "OpenGL extensions:";
        while(it != end_it) {
            MGE_INFO_LOG(OPENGL) << *it;
            ++it;
        }
    }

    void
    window_render_context::assign_thread()
    {
        if (!wglMakeCurrent(m_hdc, m_hglrc)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(wglMakeCurrent);
        }
    }

    void
    window_render_context::flush()
    {
        if(!SwapBuffers(m_hdc)) {
            MGE_THROW_CURRENT_SYSTEM_ERROR << MGE_CALLED_FUNCTION(SwapBuffers);
        }
    }
#endif
}

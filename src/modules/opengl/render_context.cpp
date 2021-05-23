#include "render_context.hpp"
#include "mge/core/system_error.hpp"
#include "mge/core/trace.hpp"
#include "mge/core/zero_memory.hpp"

namespace mge {

    MGE_USE_TRACE(OPENGL);

    namespace opengl {
#ifdef MGE_OS_WINDOWS
        render_context::render_context(mge::opengl::window *context_window)
            : m_hwnd(context_window->hwnd()), m_hdc(0), m_hglrc(0)
        {
            m_hdc = GetDC(m_hwnd);
            if (!m_hdc) {
                MGE_THROW(system_error) << MGE_CALLED_FUNCTION(GetDC);
            }

            select_pixel_format();
            create_glrc();
            init_gl3w();
            void collect_opengl_info();
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
                MGE_THROW(system_error)
                    << MGE_CALLED_FUNCTION(ChoosePixelFormat);
            }
            if (!SetPixelFormat(m_hdc, pixel_format, &pixel_format_desc)) {
                MGE_THROW(system_error) << MGE_CALLED_FUNCTION(SetPixelFormat);
            }
        }

        void render_context::create_glrc()
        {
            m_hglrc = wglCreateContext(m_hdc);
            if (!m_hglrc) {
                MGE_THROW(system_error)
                    << MGE_CALLED_FUNCTION(wglCreateContext);
            }
            if (!wglMakeCurrent(m_hdc, m_hglrc)) {
                MGE_THROW(system_error) << MGE_CALLED_FUNCTION(wglMakeCurrent);
            }
        }

        static bool s_gl3w_initialized = false;

        void render_context::init_gl3w()
        {
            if (!s_gl3w_initialized) {
                MGE_INFO_TRACE(OPENGL) << "Initializing gl3w";
                auto rc = gl3wInit();
                if (rc) {
                    MGE_THROW(runtime_exception)
                        << MGE_CALLED_FUNCTION(gl3wInit)
                        << "Initializing gl32 library failed: " << rc;
                }
                s_gl3w_initialized = true;
            }
        }

        void render_context::collect_opengl_info() {}

#else
#    error Missing port
#endif

        render_context::~render_context() {}

    } // namespace opengl
} // namespace mge
#include "opengl.hpp"
#include "window.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/render_context.hpp"

namespace opengl {
    class window_render_context : public mge::render_context
    {
    public:
        window_render_context(window *w);
        ~window_render_context();


#ifdef MGE_OS_WINDOWS
    private:
        void choose_pixel_format();
        void create_glrc();
        void init_gl3w();
        void collect_opengl_info();

        HWND  m_hwnd;
        HDC   m_hdc;
        HGLRC m_hglrc;
#endif
    };
}

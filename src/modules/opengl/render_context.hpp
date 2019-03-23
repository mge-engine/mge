#include "opengl.hpp"
#include "window.hpp"
#include "mge/config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/render_context.hpp"

namespace opengl {
    class render_context : public mge::render_context
    {
    public:
        render_context(window *w);
        ~render_context();

#ifdef MGE_OS_WINDOWS
        void assign_thread() override;
        void flush() override;
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

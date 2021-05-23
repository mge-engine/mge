#include "mge/config.hpp"
#include "mge/graphics/render_context.hpp"
#include "opengl.hpp"
#include "window.hpp"

namespace mge {

    namespace opengl {
        class render_context : public mge::render_context
        {
        public:
            render_context(mge::opengl::window *context_window);
            virtual ~render_context();
#ifdef MGE_OS_WINDOWS
        private:
            void select_pixel_format();
            void create_glrc();
            void init_gl3w();
            void collect_opengl_info();

            HWND  m_hwnd;
            HDC   m_hdc;
            HGLRC m_hglrc;
#else
#    error Missing port
#endif
        };
    } // namespace opengl
} // namespace mge
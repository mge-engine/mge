#include "mge/config.hpp"
#include "mge/graphics/render_context.hpp"
#include "window.hpp"
namespace mge {

    namespace opengl {
        class render_context : public mge::render_context
        {
        public:
            render_context(mge::opengl::window *context_window);
            virtual ~render_context();
        };
    } // namespace opengl
} // namespace mge
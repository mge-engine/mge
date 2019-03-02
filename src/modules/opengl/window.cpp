#include "window.hpp"

namespace opengl {

    window::window(const mge::rectangle& rect,
                   const mge::window_options& options)
        :platform::window(rect, options)
    {}

    window::~window()
    {}

}

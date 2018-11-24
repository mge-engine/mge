#include "mge/graphics/window.hpp"
namespace mge {
    window::window(const rectangle &rect,
                   const window_options &options)
        :m_rect(rect)
        ,m_options(options)
    {}

    window::~window()
    {}
}

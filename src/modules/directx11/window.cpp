#include "window.hpp"
namespace dx11 {
    window::window(const mge::rectangle &rect,
                   const mge::window_options &options,
                   const system_config &config)
        :platform::window(rect, options)
    {
        create_render_context(config);
    }

    window::~window()
    {}

    void
    window::create_render_context(const system_config &config)
    {

    }


}

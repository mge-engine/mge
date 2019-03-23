#include "render_context.hpp"
namespace dx11 {

    render_context::render_context(async_executor *display_executor,
                                   const system_config &config)
        : mge::render_context(display_executor)
    {}

    render_context::~render_context()
    {}

}

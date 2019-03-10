#include "mge/graphics/render_target.hpp"
namespace mge {
    render_target::render_target(async_executor *display_executor)
        :m_display_executor(display_executor)
    {}

    void
    render_target::await(const std::function<void()>& f)
    {
        m_display_executor->await(f);
    }


}

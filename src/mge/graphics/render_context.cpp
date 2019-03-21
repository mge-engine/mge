#include "mge/graphics/render_context.hpp"
namespace mge {
    render_context::render_context(async_executor *display_executor)
        :m_display_executor(display_executor)
    {}

    void
    render_context::await(const std::function<void()>& f)
    {
        m_display_executor->await(f);
    }

    void
    render_context::assign_thread()
    {}


}

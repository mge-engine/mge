#include "mge/application/application.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"

using namespace mge;

MGE_DEFINE_LOG(BLACKSCREEN);

class blackscreen : public application
{
public:
    blackscreen() = default;

    void setup() override
    {
        MGE_DEBUG_LOG(BLACKSCREEN) << "Setup blackscreen";
        m_render_system = render_system::create();
        m_window = m_render_system->create_window();
        set_quit();
    }

private:
    render_system_ref m_render_system;
    window_ref        m_window;
};

MGE_REGISTER_IMPLEMENTATION(blackscreen, mge::application, blackscreen);

MGE_MAINFUNCTION

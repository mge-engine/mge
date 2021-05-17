#include "mge/application/application.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/render_system.hpp"

MGE_DEFINE_TRACE(BLACKSCREEN);

namespace mge {
    class blackscreen : public application
    {
    public:
        blackscreen() = default;

        void setup() override
        {
            MGE_DEBUG_TRACE(BLACKSCREEN) << "Setup blackscreen";
            /*

            m_render_system = render_system::create();
            m_window        = m_render_system->create_window();
            m_window->set_close_listener([&] { set_quit(); });
            m_window->set_key_action_handler(
                [&](mge::key k, mge::key_action a, mge::modifier m) {
                    if (a == mge::key_action::PRESS && k == mge::key::ESCAPE) {
                        set_quit();
                    }
                });

            m_clear_commands = m_window->render_context().create_command_list();
            m_clear_commands->clear(rgba_color(0.0f, 0.0f, 0.0f, 1.0f));
            m_clear_commands->finish();
            m_window->set_redraw_listener([&](window::redraw_context &context) {
                context.render_context.execute(m_clear_commands);
                context.render_context.flush();
            });
            m_window->show();
            */
        }

    private:
        render_system_ref m_render_system;
        window_ref        m_window;
    };

    MGE_REGISTER_IMPLEMENTATION(blackscreen, mge::application, blackscreen);
} // namespace mge

MGE_MAINFUNCTION
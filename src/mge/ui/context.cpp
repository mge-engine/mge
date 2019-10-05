#include "mge/ui/context.hpp"
namespace mge {
    namespace ui {
        context::context()
            :m_input(nullptr)
        {}

        context::~context()
        {
            detach_input();
        }

        void
        context::draw(mge::render_context& render_context)
        {
        }

        void
        context::attach_input(mge::input_handler& input)
        {
            if(m_input) {
                detach_input();
            }
            m_input = &input;
        }

        void
        context::detach_input()
        {
            m_input = nullptr;
        }
    }
}
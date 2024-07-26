// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
#include "mge/core/parameter.hpp"
#include "mge/graphics/frame_command_list.hpp"

namespace mge {
    render_context::render_context() {}

    const swap_chain_ref& render_context::swap_chain() const
    {
        return m_swap_chain;
    }

    frame_command_list_ref render_context::create_current_frame_command_list()
    {
        class delegating_frame_command_list : public frame_command_list
        {
        public:
            delegating_frame_command_list(const command_list_ref& l)
                : frame_command_list(l->context(),
                                     frame_command_list::NO_BACKBUFFER,
                                     l->native())
                , m_command_list(l)
            {}

            ~delegating_frame_command_list() {}

            void clear(const rgba_color& c) override
            {
                m_command_list->clear(c);
            }

            void draw(const draw_command& command) override
            {
                m_command_list->draw(command);
            }

            void execute() override { m_command_list->execute(); }
            void finish() override { m_command_list->finish(); }

        private:
            command_list_ref m_command_list;
        };

        return std::make_shared<delegating_frame_command_list>(
            create_command_list());
    }

} // namespace mge
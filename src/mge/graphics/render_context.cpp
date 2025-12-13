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

    frame_command_list* render_context::create_current_frame_command_list()
    {
        class delegating_frame_command_list : public frame_command_list
        {
        public:
            delegating_frame_command_list(command_list* l)
                : frame_command_list(l->context(),
                                     frame_command_list::NO_BACKBUFFER,
                                     l->native())
                , m_command_list(l)
            {}

            ~delegating_frame_command_list()
            {
                if (m_command_list) {
                    m_command_list->destroy();
                }
            }

            void viewport(const mge::viewport& vp) override
            {
                m_command_list->viewport(vp);
            }

            void scissor(const rectangle& rect) override
            {
                m_command_list->scissor(rect);
            }

            void default_scissor() override
            {
                m_command_list->default_scissor();
            }

            void clear(const rgba_color& c) override
            {
                m_command_list->clear(c);
            }

            void clear_depth(float depth) override
            {
                m_command_list->clear_depth(depth);
            }

            void clear_stencil(int32_t stencil) override
            {
                m_command_list->clear_stencil(stencil);
            }

            void draw(const draw_command& command) override
            {
                m_command_list->draw(command);
            }

            void execute() override
            {
                m_command_list->execute();
            }
            void finish() override
            {
                m_command_list->finish();
            }

        private:
            command_list* m_command_list;
        };

        auto ptr = std::make_unique<delegating_frame_command_list>(
            create_command_list());
        auto* result = ptr.release();
        return result;
    }

} // namespace mge
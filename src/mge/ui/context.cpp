// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/context.hpp"
namespace mge {
    namespace ui {
        context::context() : m_input(nullptr) {}

        context::~context() { detach_input(); }

        void context::on_draw(mge::render_context &render_context) {}

        void context::attach_input(mge::input_handler &input)
        {
            if (m_input) {
                detach_input();
            }
            m_input = &input;
        }

        void context::detach_input() { m_input = nullptr; }

    } // namespace ui
} // namespace mge

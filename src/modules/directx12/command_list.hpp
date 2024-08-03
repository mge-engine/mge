// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/graphics/command_list.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

#include <tuple>

namespace mge::dx12 {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void scissor(const mge::rectangle& rect) override;
        void viewport(const mge::viewport& vp) override;
        void default_scissor() override;
        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void finish() override;
        void execute() override;

    private:
        render_context&                          m_dx12_context;
        mge::small_vector<mge::draw_command, 16> m_draw_commands;
        rgba_color                               m_clear_color;
        mge::rectangle                           m_scissor;
        mge::viewport                            m_viewport;
        bool                                     m_color_set;
    };
} // namespace mge::dx12
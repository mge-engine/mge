// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/small_vector.hpp"
#include "mge/graphics/command_list.hpp"
#include "render_context.hpp"

#include <tuple>

namespace mge::vulkan {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void scissor(const mge::rectangle& rect) override;
        void viewport(const mge::viewport& vp) override;
        void default_scissor() override;
        void clear(const rgba_color& c) override;
        void clear_depth(float depth) override;
        void clear_stencil(int32_t stencil) override;
        void draw(const mge::draw_command& command) override;
        void finish() override;
        void execute() override;

    private:
        render_context&                          m_vulkan_context;
        mge::small_vector<mge::draw_command, 16> m_draw_commands;
        rgba_color                               m_clear_color;
        float                                    m_clear_depth;
        int32_t                                  m_clear_stencil;
        mge::rectangle                           m_scissor;
        mge::viewport                            m_viewport;

        struct properties_set
        {
            uint32_t clear_color : 1;
            uint32_t clear_depth : 1;
            uint32_t clear_stencil : 1;
        };

        properties_set m_properties_set;
    };
} // namespace mge::vulkan
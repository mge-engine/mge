// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void viewport(const mge::viewport& vp) override;
        void scissor(const rectangle& rect) override;
        void default_scissor() override;

        void clear(const rgba_color& c) override;
        void clear_depth(float depth) override;
        void clear_stencil(int32_t stencil) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;
        void finish() override;

    private:
        bool is_recording() const
        {
            return m_deferred_context.get() != nullptr;
        }

        void start_recording();

        render_context&                     m_dx11_context;
        com_unique_ptr<ID3D11DeviceContext> m_deferred_context;
        com_unique_ptr<ID3D11CommandList>   m_command_list;
    };

} // namespace mge::dx11
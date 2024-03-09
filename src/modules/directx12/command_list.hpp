// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/command_list.hpp"
#include "mge/win32/com_unique_ptr.hpp"
#include "render_context.hpp"

namespace mge::dx12 {

    class command_list : public mge::command_list
    {
    public:
        command_list(render_context& context);
        ~command_list();

        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;

    private:
        render_context&                    m_dx12_context;
        com_ptr<ID3D12CommandAllocator>    m_bundle_allocator;
        com_ptr<ID3D12GraphicsCommandList> m_command_list;
        rgba_color                         m_clear_color;
        bool                               m_color_set;
    };

} // namespace mge::dx12
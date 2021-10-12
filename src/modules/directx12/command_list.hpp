// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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
        void execute() override;

    private:
        render_context&                           m_dx12_context;
        com_unique_ptr<ID3D12GraphicsCommandList> m_command_list;
    };

} // namespace mge::dx12
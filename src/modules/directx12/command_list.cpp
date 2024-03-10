// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "command_list.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::dx12 {
#include <d3d12.h> // Include the necessary header file

    command_list::command_list(render_context& context)
        : mge::command_list(context, true)
        , m_dx12_context(context)
        , m_color_set(false)
    {
        HRESULT rc = m_dx12_context.device()->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_BUNDLE,
            IID_PPV_ARGS(&m_bundle_allocator));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandAllocator);

        rc = m_dx12_context.device()->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_BUNDLE,
            m_bundle_allocator.Get(),
            nullptr,
            IID_PPV_ARGS(&m_command_list));
        CHECK_HRESULT(rc, ID3D12Device, CreateCommandList);
    }

    command_list::~command_list() {}

    void command_list::clear(const rgba_color& c)
    {
        m_color_set = true;
        m_clear_color = c;
    }

    void command_list::draw(const mge::draw_command& command) {}

    void command_list::execute() { m_dx12_context.execute(*this); }

    bool command_list::empty() const { return true; }

} // namespace mge::dx12
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

        void clear(const rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;

        inline bool              color_set() const { return m_color_set; }
        inline const rgba_color& clear_color() const { return m_clear_color; }

        bool empty() const;

    private:
        void release_draw_list();

        render_context&                 m_dx12_context;
        com_ptr<ID3D12CommandAllocator> m_bundle_allocator;

        using draw_data = std::tuple<ID3D12RootSignature*,
                                     ID3D12PipelineState*,
                                     ID3D12CommandList*>;
        using draw_list = small_vector<draw_data, 4>;

        draw_list             m_draw_list;
        rgba_color            m_clear_color;
        D3D12_RASTERIZER_DESC m_rasterizer_desc;
        D3D12_BLEND_DESC      m_blend_desc;
        bool                  m_color_set;
    };

    inline command_list& dx12_command_list(mge::command_list& cl)
    {
        return static_cast<command_list&>(cl);
    }

    inline const command_list& dx12_command_list(const mge::command_list& cl)
    {
        return static_cast<const command_list&>(cl);
    }

} // namespace mge::dx12
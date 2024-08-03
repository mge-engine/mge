#pragma once
#include "dx12.hpp"
#include "mge/graphics/frame_command_list.hpp"
#include "mge/win32/com_ptr.hpp"

namespace mge::dx12 {
    class render_context;
    class frame_command_list : public mge::frame_command_list
    {
    public:
        frame_command_list(render_context& context, uint32_t backbuffer_index);
        ~frame_command_list();

        void viewport(const mge::viewport& vp) override;
        void scissor(const mge::rectangle& r) override;
        void default_scissor() override;
        void clear(const mge::rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;
        void finish() override;

    private:
        render_context&                         m_dx12_context;
        mge::com_ptr<ID3D12CommandAllocator>    m_command_allocator;
        mge::com_ptr<ID3D12GraphicsCommandList> m_command_list;
        D3D12_RASTERIZER_DESC                   m_rasterizer_desc;
        D3D12_BLEND_DESC                        m_blend_desc;
    };
} // namespace mge::dx12
#pragma once
#include "dx12.hpp"
#include "mge/graphics/frame_command_list.hpp"

namespace mge::dx12 {
    class render_context;
    class frame_command_list : public mge::frame_command_list
    {
    public:
        frame_command_list(render_context& context,
                           uint32_t        backbuffer_index,
                           bool            native);
        ~frame_command_list() = default;

        void clear(const mge::rgba_color& c) override;
        void draw(const mge::draw_command& command) override;
        void execute() override;
        void finish() override;

    private:
        render_context& m_dx12_context;
    };
} // namespace mge::dx12
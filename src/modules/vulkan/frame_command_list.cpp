#include "frame_command_list.hpp"
#include "render_context.hpp"

namespace mge::vulkan {
    frame_command_list::frame_command_list(render_context& context,
                                           uint32_t        backbuffer_index)
        : mge::frame_command_list(context, backbuffer_index, true)
        , m_vulkan_context(context)
    {}

    frame_command_list::~frame_command_list() {}

    void frame_command_list::clear(const mge::rgba_color& c)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void frame_command_list::draw(const mge::draw_command& command)
    {
        MGE_THROW_NOT_IMPLEMENTED;
    }

    void frame_command_list::execute() { MGE_THROW_NOT_IMPLEMENTED; }

    void frame_command_list::finish() { MGE_THROW_NOT_IMPLEMENTED; }
} // namespace mge::vulkan
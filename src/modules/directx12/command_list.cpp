#include "command_list.hpp"
#include "render_context.hpp"
#include "mge/graphics/render_context.hpp"

namespace dx12 {
    command_list::command_list(render_context& context)
        :mge::command_list(context)
    {}

    command_list::~command_list()
    {}

    void
    command_list::on_finish()
    {}

    void
    command_list::clear(const mge::rgba_color& c)
    {}

}
#include "mge/graphics/memory_command_list.hpp"
namespace mge {

    memory_command_list::memory_command_list(render_context &context)
        :command_list(context, false)
    {}

    memory_command_list::~memory_command_list()
    {}
}

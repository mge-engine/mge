#include "command_sequence.hpp"
#include "render_context.hpp"

namespace mge::opengl {

    command_sequence::command_sequence(render_context& context)
        : mge::command_sequence(context)
    {}

    command_sequence::~command_sequence() = default;

} // namespace mge::opengl
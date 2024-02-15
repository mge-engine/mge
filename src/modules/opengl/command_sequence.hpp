#pragma once
#include "mge/graphics/command_sequence.hpp"
#include "opengl.hpp"

namespace mge::opengl {

    class render_context;

    class command_sequence : public mge::command_sequence
    {
    public:
        command_sequence(render_context& context);
        ~command_sequence() override;
    };

} // namespace mge::opengl
#include "mge/graphics/shader.hpp"

namespace mge {

    shader::shader(render_context& context, shader_type t)
        : context_object(context)
        , m_type(t)
    {}

} // namespace mge
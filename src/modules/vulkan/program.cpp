#include "program.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    program::program(render_context& context)
        : mge::program(context)
    {}

    program::~program() {}

    void program::on_link() {}

    void program::on_set_shader(const shader_ref& shader) {}

} // namespace mge::vulkan
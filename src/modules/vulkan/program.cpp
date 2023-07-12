#include "program.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    program::program(render_context& context)
        : mge::program(context)
        , m_program(nullptr)
    {
        m_program = glslang_program_create();
    }

    program::~program()
    {
        if (m_program) {
            glslang_program_delete(m_program);
        }
    }

    void program::on_link() {}

    void program::on_set_shader(const shader_ref& shader) {}

} // namespace mge::vulkan
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

    void program::on_link()
    {
        if (!glslang_program_link(m_program,
                                  GLSLANG_MSG_SPV_RULES_BIT |
                                      GLSLANG_MSG_VULKAN_RULES_BIT)) {
            const char* info_log = glslang_program_get_info_log(m_program);
            const char* debug_log =
                glslang_program_get_info_debug_log(m_program);
            MGE_ERROR_TRACE(VULKAN) << "Failed to link program: " << info_log;
            MGE_DEBUG_TRACE(VULKAN) << "Program link message: " << debug_log;
            MGE_THROW(mge::vulkan::error)
                << "Failed to link program: " << info_log;
        }
    }

    void program::on_set_shader(const shader_ref& shader)
    {
        mge::vulkan::shader* s =
            static_cast<mge::vulkan::shader*>(shader.get());
        glslang_program_add_shader(m_program, s->glslang_shader());
    }

} // namespace mge::vulkan
#include "shader.hpp"
#include "error.hpp"
#include "glslang.hpp"
#include "mge/core/on_leave.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(VULKAN);
}

namespace mge::vulkan {

    shader::shader(render_context& context, shader_type type)
        : mge::shader(context, type)
        //        , m_vulkan_context(context)
        , m_shader(nullptr)
    {}

    shader::~shader()
    {
        if (m_shader) {
            glslang_shader_delete(m_shader);
        }
    }

    glslang_stage_t shader::stage() const
    {
        switch (type()) {
        case shader_type::VERTEX:
            return GLSLANG_STAGE_VERTEX;
        case shader_type::FRAGMENT:
            return GLSLANG_STAGE_FRAGMENT;
        case shader_type::GEOMETRY:
            return GLSLANG_STAGE_GEOMETRY;
        case shader_type::TESSELATION_CONTROL:
            return GLSLANG_STAGE_TESSCONTROL;
        case shader_type::TESSELATION_EVALUATION:
            return GLSLANG_STAGE_TESSEVALUATION;
        case shader_type::COMPUTE:
            return GLSLANG_STAGE_COMPUTE;
        default:
            MGE_THROW(mge::vulkan::error)
                << "Unsupported shader type " << type();
        }
    }

    static const glslang_resource_t* glslang_default_resource(void)
    {
        return reinterpret_cast<const glslang_resource_t*>(
            GetDefaultResources());
    }

    void shader::on_compile(std::string_view code)
    {
        std::string code_str(code.begin(), code.end());

        const glslang_input_t input = {
            .language = GLSLANG_SOURCE_GLSL,
            .stage = stage(),
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_3,
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_6,
            .code = code_str.c_str(),
            .default_version = 100,
            .default_profile = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_ENHANCED,
            .resource = glslang_default_resource(),
        };

        m_shader = glslang_shader_create(&input);
        if (!m_shader) {
            MGE_THROW(mge::vulkan::error) << "Failed to create shader";
        }
        on_leave cleanup([&]() {
            if (m_shader) {
                glslang_shader_delete(m_shader);
            }
            m_shader = nullptr;
        });

        if (!glslang_shader_preprocess(m_shader, &input)) {
            const char* info_log = glslang_shader_get_info_log(m_shader);
            MGE_ERROR_TRACE(VULKAN) << "Fail to preprocess shader:" << info_log;

            MGE_THROW(mge::vulkan::error)
                << "Failed to preprocess shader: " << info_log;
        }

        if (!glslang_shader_parse(m_shader, &input)) {
            const char* info_log = glslang_shader_get_info_log(m_shader);
            MGE_ERROR_TRACE(VULKAN) << "Fail to parse shader:" << info_log;

            MGE_THROW(mge::vulkan::error)
                << "Failed to parse shader: " << info_log;
        }
    }

    void shader::on_set_code(const mge::buffer& code)
    {
        /*
        VkShaderModuleCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        CHECK_VK_CALL(
            m_vulkan_context.vkCreateShaderModule(m_vulkan_context.device(),
                                                  &create_info,
                                                  nullptr,
                                                  &m_module));
        */
    }
} // namespace mge::vulkan
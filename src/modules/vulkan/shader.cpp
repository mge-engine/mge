// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
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
        , m_vulkan_context(context)
        , m_shader_module(VK_NULL_HANDLE)
    {
        MGE_DEBUG_TRACE_STREAM(VULKAN) << "Create shader of type " << type;
    }

    shader::~shader()
    {
        destroy_shader_module();
    }

    const VkPipelineShaderStageCreateInfo& shader::pipeline_stage_info() const
    {
        return m_pipeline_stage_info;
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
            .resource = glslang_default_resource()};

        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_shader_create";
        auto glsl_shader = glslang_shader_create(&input);
        if (!glsl_shader) {
            MGE_THROW(mge::vulkan::error) << "Failed to create shader";
        }
        on_leave cleanup_shader([&]() {
            if (glsl_shader) {
                glslang_shader_delete(glsl_shader);
            }
            glsl_shader = nullptr;
        });
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_shader_preprocess";
        if (!glslang_shader_preprocess(glsl_shader, &input)) {
            const char* info_log = glslang_shader_get_info_log(glsl_shader);
            MGE_ERROR_TRACE_STREAM(VULKAN) << "Fail to preprocess shader:" << info_log;

            MGE_THROW(mge::vulkan::error)
                << "Failed to preprocess shader: " << info_log;
        }
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_shader_parse";
        if (!glslang_shader_parse(glsl_shader, &input)) {
            const char* info_log = glslang_shader_get_info_log(glsl_shader);
            MGE_ERROR_TRACE_STREAM(VULKAN) << "Fail to parse shader:" << info_log;

            MGE_THROW(mge::vulkan::error)
                << "Failed to parse shader: " << info_log;
        }

        auto glsl_program = glslang_program_create();
        if (!glsl_program) {
            MGE_THROW(mge::vulkan::error) << "Failed to create program";
        }
        on_leave cleanup_program([&]() {
            if (glsl_program) {
                glslang_program_delete(glsl_program);
            }
            glsl_program = nullptr;
        });
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_prohram_add_shader";
        glslang_program_add_shader(glsl_program, glsl_shader);
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_program_link";
        if (!glslang_program_link(glsl_program,
                                  GLSLANG_MSG_SPV_RULES_BIT |
                                      GLSLANG_MSG_VULKAN_RULES_BIT)) {
            const char* info_log = glslang_program_get_info_log(glsl_program);
            const char* debug_log =
                glslang_program_get_info_debug_log(glsl_program);
            MGE_ERROR_TRACE_STREAM(VULKAN) << "Failed to link program: " << info_log;
            MGE_DEBUG_TRACE_STREAM(VULKAN)
                << "Program link message: " << debug_log;
            MGE_THROW(mge::vulkan::error)
                << "Failed to link program: " << info_log;
        }
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_program_SPIRV_generate";
        glslang_program_SPIRV_generate(glsl_program, stage());
        size_t code_size_words = glslang_program_SPIRV_get_size(glsl_program);
        if (code_size_words == 0) {
            MGE_THROW(mge::vulkan::error) << "Failed to generate SPIR-V code";
        }
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Compile shader: glslang_program_SPIRV_get";
        m_code.resize(code_size_words * 4);
        glslang_program_SPIRV_get(
            glsl_program,
            reinterpret_cast<unsigned int*>(m_code.data()));
        MGE_DEBUG_TRACE_STREAM(VULKAN)
            << "Shader code size: " << code_size_words * 4 << " bytes";
        create_shader_module();
    }

    void shader::on_set_code(const mge::buffer& code)
    {
        m_code = code;
        create_shader_module();
    }

    VkShaderStageFlagBits shader::vk_stage_flags() const
    {
        switch (type()) {
        case shader_type::VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;
        case shader_type::FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        default:
            MGE_THROW(mge::vulkan::error)
                << "Unsupported shader type " << type();
        }
    }

    void shader::create_shader_module()
    {
        MGE_DEBUG_TRACE_STREAM(VULKAN) << "Create shader module";
        VkShaderModuleCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = m_code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(m_code.data());

        CHECK_VK_CALL(
            m_vulkan_context.vkCreateShaderModule(m_vulkan_context.device(),
                                                  &create_info,
                                                  nullptr,
                                                  &m_shader_module));

        m_main_function = get_property("main_function", "main");

        m_pipeline_stage_info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = vk_stage_flags(),
            .module = m_shader_module,
            .pName = m_main_function.c_str()};
    }

    void shader::destroy_shader_module()
    {
        if (m_shader_module) {
            m_vulkan_context.vkDestroyShaderModule(m_vulkan_context.device(),
                                                   m_shader_module,
                                                   nullptr);
            m_shader_module = VK_NULL_HANDLE;
        }
    }
} // namespace mge::vulkan
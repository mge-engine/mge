#include "shader.hpp"
#include "error.hpp"
#include "glslang.hpp"
#include "render_context.hpp"

namespace mge::vulkan {

    shader::shader(render_context& context, shader_type type)
        : mge::shader(context, type)
    //        , m_vulkan_context(context)
    //        , m_module(VK_NULL_HANDLE)
    {}

    shader::~shader()
    {
        /*
        if (m_module != VK_NULL_HANDLE) {
            m_vulkan_context.vkDestroyShaderModule(m_vulkan_context.device(),
                                                   m_module,
                                                   nullptr);
        }
        */
    }

    void shader::on_compile(std::string_view code)
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
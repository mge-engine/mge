#pragma once
#include "mge/graphics/shader.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;
    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

        VkShaderModule module() const noexcept { return m_module; }

    private:
        void create_shader();

        render_context& m_vulkan_context;
        VkShaderModule  m_module;
    };
} // namespace mge::vulkan
#pragma once
#include "glslang.hpp"
#include "mge/graphics/shader.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;
    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();
        inline glslang_shader_t* glslang_shader() const noexcept
        {
            return m_shader;
        }

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

    private:
        glslang_stage_t stage() const;

        glslang_shader_t* m_shader;
    };
} // namespace mge::vulkan
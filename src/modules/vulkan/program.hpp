#pragma once
#include "mge/graphics/program.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_context;
    class shader;
    class program : public mge::program
    {
    public:
        program(render_context& context);
        ~program();

    protected:
        virtual void on_link() override;
        virtual void on_set_shader(const shader_ref& shader) override;

        std::vector<VkPipelineShaderStageCreateInfo>      m_shader_stage_infos;
        std::vector<std::shared_ptr<mge::vulkan::shader>> m_shaders;
    };
} // namespace mge::vulkan
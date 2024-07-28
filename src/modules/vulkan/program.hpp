#pragma once
#include "mge/graphics/program.hpp"

namespace mge::vulkan {
    class render_context;
    class program : public mge::program
    {
    public:
        program(render_context& context);
        ~program();

    protected:
        virtual void on_link() override;
        virtual void on_set_shader(const shader_ref& shader) override;
    };
} // namespace mge::vulkan
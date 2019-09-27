#pragma once
#include "dx11.hpp"
#include "mge/graphics/pipeline.hpp"
namespace dx11 {
    
    class pipeline : public mge::pipeline
    {
    public:
        pipeline(mge::render_context& context);
        virtual ~pipeline();
    protected:
        void on_link() override;
        void on_set_shader(const mge::shader_ref& shader) override;
    };

}
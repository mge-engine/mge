#pragma once
#include "dx11.hpp"
#include "mge/graphics/pipeline.hpp"
#include <array>
namespace dx11 {
    
    class pipeline : public mge::pipeline
    {
    public:
        pipeline(mge::render_context& context);
        virtual ~pipeline();
    protected:
        void on_link() override;
        void on_set_shader(const mge::shader_ref& shader) override;
    private:
        std::array<mge::shader_ref, (size_t)mge::shader_type::MAX_SHADER_TYPE> m_shaders;
    };

}
// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/graphics/pipeline.hpp"
#include "shader.hpp"

#include <array>

namespace mge::dx11 {

    class pipeline : public mge::pipeline
    {
    public:
        pipeline(render_context& context);
        virtual ~pipeline();

    protected:
        void on_link() override;
        void on_set_shader(const shader_ref& shader) override;

    private:
        std::array<shader_ref, 6> m_shaders;
    };

} // namespace mge::dx11
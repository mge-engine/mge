// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader_type.hpp"
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
        inline const shader_ref& pipeline_shader(mge::shader_type t) const
        {
            return m_shaders[mge::to_underlying(t)];
        }

        std::array<shader_ref,
                   mge::to_underlying(mge::shader_type::MAX_SHADER_TYPE) + 1>
            m_shaders;
    };

} // namespace mge::dx11
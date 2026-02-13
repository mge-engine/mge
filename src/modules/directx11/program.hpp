// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "dx11_fwd.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader_type.hpp"
#include "shader.hpp"

#include <array>

namespace mge::dx11 {

    class program : public mge::program
    {
    public:
        program(render_context& context);
        virtual ~program();

        inline shader* program_shader(mge::shader_type t) const
        {
            return m_shaders[mge::to_underlying(t)];
        }

        uint32_t buffer_bind_point(const std::string& name) const;

    protected:
        void on_link() override;
        void on_set_shader(mge::shader* shader) override;

    private:
        void collect_information();

        std::array<shader*,
                   mge::to_underlying(mge::shader_type::MAX_SHADER_TYPE) + 1>
            m_shaders{};
    };

} // namespace mge::dx11
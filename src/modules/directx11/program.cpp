// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    program::program(render_context& context)
        : ::mge::program(context)
    {}

    program::~program() {}

    void program::on_link()
    {
        if (!program_shader(mge::shader_type::VERTEX) ||
            !program_shader(mge::shader_type::FRAGMENT)) {
            MGE_THROW(mge::illegal_state) << "Cannot link empty program";
        }
        collect_information();
    }

    void program::on_set_shader(mge::shader* s)
    {
        auto* dx11_shader = static_cast<shader*>(s);
        auto  index = mge::to_underlying(s->type());
        m_shaders[index] = dx11_shader;
    }

    void program::collect_information()
    {
        for (const auto& s : m_shaders) {
            if (s) {
                const dx11::shader* dx11_s =
                    static_cast<const dx11::shader*>(s);
                dx11_s->reflect(m_attributes, m_uniforms, m_uniform_buffers);
            }
        }
    }

    uint32_t program::buffer_bind_point(const std::string& name) const
    {
        for (const auto& ub : m_uniform_buffers) {
            if (ub.name == name) {
                return ub.location;
            }
        }
        MGE_WARNING_TRACE(DX11,
                          "Uniform buffer '{}' not found, using slot 0",
                          name);
        return 0;
    }

} // namespace mge::dx11
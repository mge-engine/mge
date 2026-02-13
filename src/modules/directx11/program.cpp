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
        for (auto& buffers : m_shader_buffers) {
            buffers.clear();
        }

        for (const auto& s : m_shaders) {
            if (s) {
                dx11::shader* dx11_s = static_cast<dx11::shader*>(s);
                dx11_s->reflect(m_attributes, m_uniforms, m_uniform_buffers);

                // Collect which buffers are used by this shader stage
                auto index = mge::to_underlying(s->type());
                for (const auto& ub : m_uniform_buffers) {
                    if (dx11_s->uses_uniform_buffer(ub.name)) {
                        m_shader_buffers[index].insert(ub.name);
                    }
                }
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

    bool program::uses_in_vertex_shader(const std::string& name) const
    {
        auto index = mge::to_underlying(mge::shader_type::VERTEX);
        return m_shader_buffers[index].find(name) !=
               m_shader_buffers[index].end();
    }

    bool program::uses_in_pixel_shader(const std::string& name) const
    {
        auto index = mge::to_underlying(mge::shader_type::FRAGMENT);
        return m_shader_buffers[index].find(name) !=
               m_shader_buffers[index].end();
    }

} // namespace mge::dx11
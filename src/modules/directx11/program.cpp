// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/core/trace.hpp"
#include "mge/slang/slang_compiler.hpp"
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
                dx11_s->reflect(m_attributes,
                                m_uniforms,
                                m_uniform_block_metadata);

                // Collect which buffers are used by this shader stage
                auto index = mge::to_underlying(s->type());
                for (const auto& ub : m_uniform_block_metadata) {
                    if (dx11_s->uses_uniform_buffer(ub.name)) {
                        m_shader_buffers[index].insert(ub.name);
                    }
                }
            }
        }
    }

    uint32_t program::buffer_bind_point(const std::string& name) const
    {
        for (const auto& ub : m_uniform_block_metadata) {
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

    void program::on_compile_and_link(const mge::shader_language& language,
                                      const std::string_view      source)
    {
        if (language.name() != shader_language::SLANG.name()) {
            MGE_THROW(mge::illegal_argument)
                << "Unsupported shader language: " << language;
        }

        auto compile_result =
            mge::slang_compile(mge::slang_target::DXBC, source);

        if (compile_result.shader_code.empty()) {
            MGE_THROW(mge::illegal_state)
                << "Slang compilation produced no shader stages";
        }

        m_owned_shaders.clear();

        for (auto& [type, shader_code] : compile_result.shader_code) {
            auto  handle = context().create_shader(type);
            auto* dx11_s = static_cast<shader*>(handle.get());
            dx11_s->set_code_immediate(shader_code.binary_code);
            m_shaders[mge::to_underlying(type)] = dx11_s;
            m_owned_shaders.push_back(handle);
        }

        // Use SLANG reflection instead of D3D reflection
        m_attributes = std::move(compile_result.attributes);
        m_uniforms = std::move(compile_result.uniforms);
        m_uniform_block_metadata = std::move(compile_result.uniform_buffers);
        m_sampler_bindings = std::move(compile_result.sampler_bindings);

        // Use per-shader D3D reflection for stage buffer tracking
        for (auto& buffers : m_shader_buffers) {
            buffers.clear();
        }
        attribute_list              dummy_attrs;
        uniform_list                dummy_uniforms;
        uniform_block_metadata_list dummy_blocks;
        for (const auto& s : m_shaders) {
            if (s) {
                dx11::shader* dx11_s = static_cast<dx11::shader*>(s);
                dx11_s->reflect(dummy_attrs, dummy_uniforms, dummy_blocks);
                auto index = mge::to_underlying(s->type());
                for (const auto& ub : m_uniform_block_metadata) {
                    if (dx11_s->uses_uniform_buffer(ub.name)) {
                        m_shader_buffers[index].insert(ub.name);
                    }
                }
            }
        }
    }

} // namespace mge::dx11
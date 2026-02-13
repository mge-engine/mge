// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/program.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/win32/com_unique_ptr.hpp"

#include <set>

namespace mge::dx11 {
    class render_context;
    class shader : public mge::shader
    {
    public:
        shader(render_context& context, shader_type type);
        ~shader();

    protected:
        virtual void on_compile(std::string_view code) override;
        virtual void on_set_code(const mge::buffer& code) override;

    public:
        using vertex_shader_t = mge::com_unique_ptr<ID3D11VertexShader>;
        using pixel_shader_t = mge::com_unique_ptr<ID3D11PixelShader>;
        using compute_shader_t = mge::com_unique_ptr<ID3D11ComputeShader>;
        using hull_shader_t = mge::com_unique_ptr<ID3D11HullShader>;
        using domain_shader_t = mge::com_unique_ptr<ID3D11DomainShader>;

        using shader_t = std::variant<vertex_shader_t,
                                      pixel_shader_t,
                                      compute_shader_t,
                                      hull_shader_t,
                                      domain_shader_t>;

        ID3D11VertexShader* directx_vertex_shader() const
        {
            return std::get<vertex_shader_t>(m_shader).get();
        }

        ID3D11PixelShader* directx_pixel_shader() const
        {
            return std::get<pixel_shader_t>(m_shader).get();
        }

        const shader_t& directx_shader() const
        {
            return m_shader;
        }
        ID3DBlob* code() const
        {
            return m_code.get();
        }
        ID3D11InputLayout* input_layout() const
        {
            return m_input_layout.get();
        }

        void
        reflect(mge::program::attribute_list&              attributes,
                mge::program::uniform_list&                uniforms,
                mge::program::uniform_block_metadata_list& uniform_buffers);

        bool uses_uniform_buffer(const std::string& name) const;

    private:
        std::string profile() const;
        void        create_shader();
        void        create_input_layout();

        shader_t                               m_shader;
        mge::com_unique_ptr<ID3DBlob>          m_code;
        mge::com_unique_ptr<ID3D11InputLayout> m_input_layout;
        std::set<std::string>                  m_uniform_buffer_names;
    };
} // namespace mge::dx11
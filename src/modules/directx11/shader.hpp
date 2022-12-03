// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/shader.hpp"
#include "mge/win32/com_unique_ptr.hpp"

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

    private:
        std::string profile() const;
        void        create_shader();

        shader_t                      m_shader;
        mge::com_unique_ptr<ID3DBlob> m_code;
    };
} // namespace mge::dx11
// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "mge/graphics/shader_program.hpp"
#include "win32/com_unique_ptr.hpp"

#include <variant>

namespace dx11 {

    class shader;
    MGE_DECLARE_REF(shader);

    class shader_program : public mge::shader_program
    {
    public:
        shader_program(mge::render_context& context,
               mge::shader_type type);
        virtual ~shader_program();

    protected:
        void on_compile(const std::string& source_code) override;
        void on_load(const mge::buffer& code) override;

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
        ID3D10Blob* code() const noexcept { return m_code.get(); }
    private:
        void create_shader();
        std::string profile() const;

        shader_t m_shader;
        mge::com_unique_ptr<ID3D10Blob> m_code;
    };


}

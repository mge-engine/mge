// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11.hpp"
#include "mge/graphics/shader.hpp"
#include "win32/com_unique_ptr.hpp"

#include <variant>

namespace dx11 {

    class shader;
    MGE_DECLARE_REF(shader);

    class shader : public mge::shader
    {
    public:
        shader(mge::render_context& context,
               mge::shader_type type);
        virtual ~shader();

    protected:
        void on_compile(const std::string& source_code) override;
        void on_load(const mge::buffer& code) override;
        
    public:
        typedef COM_UNIQUE_PTR(ID3D11VertexShader) vertex_shader_t;
        typedef COM_UNIQUE_PTR(ID3D11PixelShader) pixel_shader_t;
        typedef COM_UNIQUE_PTR(ID3D11ComputeShader) compute_shader_t;
        typedef COM_UNIQUE_PTR(ID3D11HullShader) hull_shader_t;
        typedef COM_UNIQUE_PTR(ID3D11DomainShader) domain_shader_t;

        typedef std::variant<vertex_shader_t,
                             pixel_shader_t,
                             compute_shader_t,
                             hull_shader_t,
                             domain_shader_t> shader_t;
        ID3D10Blob* code() const noexcept { return m_code.get(); }
    private:
        void create_shader();
        std::string profile() const;

        shader_t m_shader;
        COM_UNIQUE_PTR(ID3D10Blob) m_code;
    };


}

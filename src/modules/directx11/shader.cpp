// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "shader.hpp"
#include "error.hpp"
#include "render_context.hpp"

namespace mge::dx11 {
    shader::shader(render_context& context, shader_type type)
        : mge::shader(context, type)
    {}

    shader::~shader() {}

    void shader::on_compile(std::string_view code)
    {
        ID3DBlob* compiled_code = nullptr;
        ID3DBlob* errors = nullptr;
        auto      file_name = get_property("file_name", "shader");
        auto      main_function = get_property("main_function", "main");
        auto      shader_profile = profile();

        auto rc = D3DCompile(code.data(),
                             code.size(),
                             file_name.c_str(),
                             nullptr,
                             nullptr,
                             main_function.c_str(),
                             shader_profile.c_str(),
                             0,
                             0,
                             &compiled_code,
                             &errors);
        if (FAILED(rc)) {
            std::string errormessage(
                static_cast<const char*>(errors->GetBufferPointer()),
                static_cast<const char*>(errors->GetBufferPointer()) +
                    errors->GetBufferSize());
            errors->Release();
            throw dx11::error().set_info_from_hresult(rc,
                                                      __FILE__,
                                                      __LINE__,
                                                      "",
                                                      "D3DCompile")
                << "(" << errormessage << ")";
        } else {
            m_code.reset(compiled_code);
        }

        create_shader();
    }

    void shader::create_shader()
    {
        render_context& ctx = dx11_context(context());
        HRESULT         rc = 0;
        switch (type()) {
        case mge::shader_type::FRAGMENT: {
            ID3D11PixelShader* pixel_shader = nullptr;
            rc = ctx.device()->CreatePixelShader(m_code->GetBufferPointer(),
                                                 m_code->GetBufferSize(),
                                                 nullptr,
                                                 &pixel_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreatePixelShader);
            m_shader = mge::make_com_unique_ptr(pixel_shader);
            break;
        }
        case mge::shader_type::VERTEX: {
            ID3D11VertexShader* vertex_shader;
            rc = ctx.device()->CreateVertexShader(m_code->GetBufferPointer(),
                                                  m_code->GetBufferSize(),
                                                  nullptr,
                                                  &vertex_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreateVertexShader);
            m_shader = mge::make_com_unique_ptr(vertex_shader);
            break;
        }
        case mge::shader_type::COMPUTE: {
            ID3D11ComputeShader* compute_shader;
            rc = ctx.device()->CreateComputeShader(m_code->GetBufferPointer(),
                                                   m_code->GetBufferSize(),
                                                   nullptr,
                                                   &compute_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreateComputeShader);
            m_shader = mge::make_com_unique_ptr(compute_shader);
            break;
        }
        case mge::shader_type::TESSELATION_CONTROL:
        case mge::shader_type::TESSELATION_EVALUATION:
        default:
            MGE_THROW(dx11::error) << "Unsupported shader type " << type();
        }
    }

    void shader::on_set_code(const mge::buffer& code)
    {
        ID3DBlob* code_blob = nullptr;
        auto      rc = D3DCreateBlob(code.size(), &code_blob);
        CHECK_HRESULT(rc, , D3DCreateBlob);
        m_code.reset(code_blob);
        memcpy(m_code->GetBufferPointer(), code.data(), code.size());
        create_shader();
    }

    std::string shader::profile() const
    {
        switch (type()) {
        case mge::shader_type::FRAGMENT:
            return get_property("profile", "ps_5_0");
        case mge::shader_type::VERTEX:
            return get_property("profile", "vs_5_0");
        case mge::shader_type::COMPUTE:
            return get_property("profile", "cs_5_0");
        default:
            MGE_THROW(dx11::error) << "Unsupported shader type: " << type();
        }
    }

} // namespace mge::dx11
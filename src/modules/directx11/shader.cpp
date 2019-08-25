#include "shader.hpp"
#include "render_context.hpp"
#include "error.hpp"

namespace dx11 {

    shader::shader(mge::render_context& context,
                   mge::shader_type type)
        :mge::shader(context, type)
    {}

    shader::~shader()
    {}

    void
    shader::compile(const std::string &source_code)
    {

        ID3DBlob *code = nullptr;
        ID3DBlob *errors = nullptr;
        std::string file_name = get_property("file_name", "shader");
        std::string main_function = get_property("main_function", "main");
        std::string shader_profile = profile();

        auto rc = D3DCompile(source_code.c_str(),
                             source_code.size(),
                             file_name.c_str(),
                             nullptr,
                             nullptr,
                             main_function.c_str(),
                             shader_profile.c_str(),
                             0,
                             0,
                             &code,
                             &errors);
        if(FAILED(rc)) {
            std::string errormessage(static_cast<const char *>(errors->GetBufferPointer()),
                                     static_cast<const char *>(errors->GetBufferPointer())
                                     + errors->GetBufferSize());
            errors->Release();
            throw dx11::error().set_info_from_hresult(rc, __FILE__, __LINE__, "", "D3DCompile")
                    << "(" << errormessage << ")";
        } else {
           m_code.reset(code);
        }

        create_shader();
    }

    void
    shader::load(const mge::buffer &code)
    {
        ID3DBlob *code_blob = nullptr;
        auto rc = D3DCreateBlob(code.size(), &code_blob);
        CHECK_HRESULT(rc, ,D3DCreateBlob);
        m_code.reset(code_blob);
        memcpy(m_code->GetBufferPointer(),
               code.data(),
               code.size());
        create_shader();
    }

    std::string
    shader::profile() const
    {
        switch(type()) {
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

    void
    shader::create_shader()
    {
        auto device = dx11_device(*context());
        HRESULT rc = 0;
        switch(type()) {
        case mge::shader_type::FRAGMENT:
        {
            ID3D11PixelShader *pixel_shader = nullptr;
            rc = device->CreatePixelShader(m_code->GetBufferPointer(),
                                           m_code->GetBufferSize(),
                                           nullptr,
                                           &pixel_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreatePixelShader);
            m_shader = mge::make_com_unique_ptr(pixel_shader);
            break;
        }
        case mge::shader_type::VERTEX:
        {
            ID3D11VertexShader *vertex_shader;
            rc = device->CreateVertexShader(m_code->GetBufferPointer(),
                                            m_code->GetBufferSize(),
                                            nullptr,
                                            &vertex_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreateVertexShader);
            m_shader = mge::make_com_unique_ptr(vertex_shader);
            break;
        }
        case mge::shader_type::COMPUTE:
        {
            ID3D11ComputeShader *compute_shader;
            rc = device->CreateComputeShader(m_code->GetBufferPointer(),
                                             m_code->GetBufferSize(),
                                             nullptr,
                                             &compute_shader);
            CHECK_HRESULT(rc, ID3D11Device, CreateComputeShader);
            m_shader = mge::make_com_unique_ptr(compute_shader);
            break;
        }
        default:
            MGE_THROW(dx11::error) << "Unsupported shader type " << type();
        }
    }
}

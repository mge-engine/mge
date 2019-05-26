#include "shader.hpp"
#include "render_context.hpp"
#include "error.hpp"

namespace dx11 {

    shader::shader(mge::render_context& context,
                   mge::shader_type type)
        :mge::shader(context, type)
    {
        //create_shader();
    }

    shader::~shader()
    {}

    void
    shader::compile(const std::string &source_code)
    {

//        if(m_shader.vertex_shader) {
//                    m_shader.vertex_shader->Release();
//                    m_shader.vertex_shader =  nullptr;
//                }
//                if(m_code_blob) {
//                    m_code_blob.release();
//                }
//                if(!m_source_code.empty()) {
//                    std::string main_function = get_property("main_function", "main");
//                    std::string profile = get_profile();
//                    std::string file_name = get_property("file_name", "shader");
//                    ID3DBlob* code=nullptr, *errors=nullptr;

//                    HRESULT rc = D3DCompile(m_source_code.c_str(),
//                                            m_source_code.size(),
//                                            file_name.c_str(),
//                                            NULL,
//                                            NULL,
//                                            main_function.c_str(),
//                                            profile.c_str(),
//                                            0,
//                                            0,
//                                            &code,
//                                            &errors);
//                    if(FAILED(rc)) {
//                        std::string errormessage(static_cast<const char *>(errors->GetBufferPointer()),
//                                                 static_cast<const char *>(errors->GetBufferPointer())
//                                                 + errors->GetBufferSize());
//                        errors->Release();
//                        throw DX11_ERROR(rc, "", "D3DCompile")
//                              << "(" << errormessage << ")";
//                    } else {
//                        m_code_blob.reset(code);
//                    }
//                } else if(!m_code.empty()) {
//                    ID3DBlob *code = nullptr;
//                    HRESULT rc = D3DCreateBlob(m_code.size(), &code);
//                    CHECK_HRESULT(rc, , D3DCreateBlob);
//                    m_code_blob.reset(code);
//                    memcpy(m_code_blob->GetBufferPointer(), &m_code[0], m_code.size());
//                } else {
//                    MOGE_THROW(moge::illegal_state) << "No source code or compiled code set in shader";
//                }
//                create_shader_with_code();

    }

    void
    shader::create_shader()
    {
//        auto device = dx11_device(*context());
//        HRESULT rc = 0;
//        switch(type()) {
//        case mge::shader_type::FRAGMENT:
//        {
//            ID3D11PixelShader *pixel_shader = nullptr;
//            rc = device->CreatePixelShader(m_code
//                                           0,
//                                           nullptr,
//                                           &pixel_shader);
//            CHECK_HRESULT(rc, ID3D11Device, CreatePixelShader);
//            m_shader = mge::make_com_unique_ptr(pixel_shader);
//            break;
//        }

//        case moge::shader_type::VERTEX:
//                        rc = dx11_device->CreateVertexShader(m_code_blob->GetBufferPointer(),
//                                                             m_code_blob->GetBufferSize(),
//                                                             nullptr,
//                                                             &m_shader.vertex_shader);
//                        CHECK_HRESULT(rc, ID3D11Device, CreateVertexShader);
//                        break;
//                    case moge::shader_type::COMPUTE:
//                        rc = dx11_device->CreateComputeShader(m_code_blob->GetBufferPointer(),
//                                                              m_code_blob->GetBufferSize(),
//                                                              nullptr,
//                                                              &m_shader.compute_shader);
//                        CHECK_HRESULT(rc, ID3D11Device, CreateComputeShader);
//                        break;

//        default:
//            MGE_THROW(dx11::error) << "Unsupported shader type " << type();
//        }
    }
}

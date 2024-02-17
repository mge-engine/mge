// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "shader.hpp"
#include "error.hpp"
#include "mge/core/on_leave.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

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
            create_input_layout();
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

    static void dump_shader_desc(const D3D11_SHADER_DESC& d)
    {
        MGE_DEBUG_TRACE(DX11) << "Version: " << d.Version;
        MGE_DEBUG_TRACE(DX11) << "Creator: " << d.Creator;
        MGE_DEBUG_TRACE(DX11) << "Flags  : " << d.Flags;
    }

    static mge::data_type
    data_type_of_parameter(const D3D11_SIGNATURE_PARAMETER_DESC& parameter_desc)
    {
        switch (parameter_desc.ComponentType) {
        case D3D_REGISTER_COMPONENT_UINT32:
            return mge::data_type::UINT32;
        case D3D_REGISTER_COMPONENT_SINT32:
            return mge::data_type::INT32;
        case D3D_REGISTER_COMPONENT_FLOAT32:
            return mge::data_type::FLOAT;
        default:
            MGE_THROW(dx11::error) << "Unsupported component type "
                                   << parameter_desc.ComponentType;
        }
    }

    static mge::data_type
    data_type_of_variable(const D3D11_SHADER_TYPE_DESC& variable_type_desc)
    {
        switch (variable_type_desc.Type) {
        case D3D_SVT_UINT:
            return mge::data_type::UINT32;
        case D3D_SVT_INT:
            return mge::data_type::INT32;
        case D3D_SVT_FLOAT:
            return mge::data_type::FLOAT;
        default:
            MGE_THROW(dx11::error)
                << "Unsupported variable type " << variable_type_desc.Type;
        }
    }

    static uint8_t
    size_of_parameter(const D3D11_SIGNATURE_PARAMETER_DESC& parameter_desc)
    {
        switch (parameter_desc.Mask) {
        case 0x1:
            return 1;
        case 0x3:
            return 2;
        case 0x7:
            return 3;
        case 0xf:
            return 4;
        case 0x0:
            return 0;
        default:
            MGE_THROW(dx11::error)
                << "Unsupported parameter mask " << (int)parameter_desc.Mask;
        }
    }

    void
    shader::reflect(mge::program::attribute_list&      attributes,
                    mge::program::uniform_list&        uniforms,
                    mge::program::uniform_buffer_list& uniform_buffers) const
    {
        ID3D11ShaderReflection* shader_reflection = nullptr;

        CHECK_HRESULT(D3DReflect(m_code->GetBufferPointer(),
                                 m_code->GetBufferSize(),
                                 IID_ID3D11ShaderReflection,
                                 (void**)&shader_reflection),
                      ,
                      D3DReflect);
        on_leave delete_shader_reflection([&]() {
            if (shader_reflection) {
                shader_reflection->Release();
            }
        });
        if (shader_reflection) {
            D3D11_SHADER_DESC shader_desc = {};
            shader_reflection->GetDesc(&shader_desc);
            dump_shader_desc(shader_desc);

            for (uint32_t i = 0; i < shader_desc.InputParameters; ++i) {
                D3D11_SIGNATURE_PARAMETER_DESC parameter_desc = {};
                shader_reflection->GetInputParameterDesc(i, &parameter_desc);
                attributes.push_back(mge::program::attribute{
                    parameter_desc.SemanticName,
                    data_type_of_parameter(parameter_desc),
                    size_of_parameter(parameter_desc)});
                MGE_DEBUG_TRACE(DX11)
                    << "attribute[" << i << "]=" << attributes.back();
            }

            for (uint32_t i = 0; i < shader_desc.ConstantBuffers; ++i) {
                ID3D11ShaderReflectionConstantBuffer* cbuffer =
                    shader_reflection->GetConstantBufferByIndex(i);

                D3D11_SHADER_BUFFER_DESC cbuffer_desc = {};
                cbuffer->GetDesc(&cbuffer_desc);
                mge::program::uniform_buffer uniform_buffer;
                uniform_buffer.name = cbuffer_desc.Name;
                for (uint32_t j = 0; j < cbuffer_desc.Variables; ++j) {
                    ID3D11ShaderReflectionVariable* variable =
                        cbuffer->GetVariableByIndex(j);
                    D3D11_SHADER_VARIABLE_DESC variable_desc = {};
                    variable->GetDesc(&variable_desc);
                    mge::program::uniform u;
                    u.name = variable_desc.Name;
                    ID3D11ShaderReflectionType* variable_type =
                        variable->GetType();
                    D3D11_SHADER_TYPE_DESC variable_type_desc = {};
                    variable_type->GetDesc(&variable_type_desc);
                    u.type = data_type_of_variable(variable_type_desc);
                    u.size = variable_desc.Size;
                    uniform_buffer.uniforms.push_back(u);
                }
                if (uniform_buffer.name == "$Globals") {
                    uniforms.insert(uniforms.begin(),
                                    uniform_buffer.uniforms.begin(),
                                    uniform_buffer.uniforms.end());
                    for (size_t k = 0; k < uniforms.size(); ++k) {
                        MGE_DEBUG_TRACE(DX11)
                            << "uniform[" << i << "]=" << uniforms[k];
                    }
                } else {
                    uniform_buffers.push_back(uniform_buffer);
                    MGE_DEBUG_TRACE(DX11)
                        << "uniform_buffer[" << uniform_buffers.size() - 1
                        << "]=" << uniform_buffer;
                }
            }
        }
    }

    void shader::create_input_layout()
    {
        D3D11_INPUT_ELEMENT_DESC input_elements[] = {
            {"POSITION",
             0,
             DXGI_FORMAT_R32G32B32_FLOAT,
             0,
             0,
             D3D11_INPUT_PER_VERTEX_DATA,
             0},
        };

        ID3D11InputLayout* input_layout = nullptr;
        render_context&    ctx = dx11_context(context());

        auto rc = ctx.device()->CreateInputLayout(input_elements,
                                                  1,
                                                  m_code->GetBufferPointer(),
                                                  m_code->GetBufferSize(),
                                                  &input_layout);
        CHECK_HRESULT(rc, ID3D11Device, CreateInputLayout);
        m_input_layout = mge::make_com_unique_ptr(input_layout);

#if 0
        ID3D11ShaderReflection* shader_reflection = nullptr;

        CHECK_HRESULT(D3DReflect(m_code->GetBufferPointer(),
                                 m_code->GetBufferSize(),
                                 IID_ID3D11ShaderReflection,
                                 (void**)&shader_reflection),
                      ,
                      D3DReflect);
        on_leave delete_shader_reflection([&]() {
            if (shader_reflection) {
                shader_reflection->Release();
            }
        });
        if (shader_reflection) {
            D3D11_SHADER_DESC shader_desc = {};
            shader_reflection->GetDesc(&shader_desc);
            dump_shader_desc(shader_desc);

            std::vector<D3D11_INPUT_ELEMENT_DESC> input_elements;
            for (uint32_t i = 0; i < shader_desc.InputParameters; ++i) {
                D3D11_SIGNATURE_PARAMETER_DESC parameter_desc = {};
                shader_reflection->GetInputParameterDesc(i, &parameter_desc);
                D3D11_INPUT_ELEMENT_DESC input_element = {};
                input_element.SemanticName = parameter_desc.SemanticName;
                input_element.SemanticIndex = parameter_desc.SemanticIndex;
                input_element.InputSlot = 0;
                input_element.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
                input_element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
                input_element.InstanceDataStepRate = 0;
                input_element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                input_elements.push_back(input_element);
            }

            HRESULT rc = dx11_context(context()).device()->CreateInputLayout(
                input_elements.data(),
                input_elements.size(),
                m_code->GetBufferPointer(),
                m_code->GetBufferSize(),
                &m_input_layout);
            CHECK_HRESULT(rc, ID3D11Device, CreateInputLayout);
        }
#endif
    }
} // namespace mge::dx11
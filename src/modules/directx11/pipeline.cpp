// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "pipeline.hpp"
#include "shader_program.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"
#include "mge/core/bit.hpp"
#include "mge/core/log_function_guard.hpp"
MGE_USE_LOG(DX11);

namespace dx11 {
    pipeline::pipeline(mge::render_context& context)
        : mge::pipeline(context)
        ,m_vertex_shader_uniform_count(0)
    {}

    pipeline::~pipeline()
    {}

    static void
    dump_shader_desc(const D3D11_SHADER_DESC& desc)
    {
        MGE_DEBUG_LOG(DX11) << "Version: " << desc.Version;
        MGE_DEBUG_LOG(DX11) << "Creator: " << desc.Creator;
        MGE_DEBUG_LOG(DX11) << "Flags  : " << desc.Flags;

        MGE_DEBUG_LOG(DX11) << "Constant Buffers  : " << desc.ConstantBuffers;
        MGE_DEBUG_LOG(DX11) << "Bound Resources   : " << desc.BoundResources;
        MGE_DEBUG_LOG(DX11) << "Input Parameters  : " << desc.InputParameters;
        MGE_DEBUG_LOG(DX11) << "Output Parameters : " << desc.OutputParameters;
    }

    static mge::data_type data_type_of_parameter(const D3D11_SIGNATURE_PARAMETER_DESC& desc)
    {
        if(desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 
           || desc.ComponentType == D3D10_REGISTER_COMPONENT_FLOAT32) {
            int count = mge::popcount(desc.Mask);
            switch(count) {
            case 1:
                return mge::data_type::FLOAT;
            case 2:
                return mge::data_type::FLOAT_VEC2;
            case 3:
                return mge::data_type::FLOAT_VEC3;
            case 4:
                return mge::data_type::FLOAT_VEC4;
            default:
                MGE_THROW(dx11::error) << "Unsupported component count " << count;
            }
        } else {
            MGE_THROW(dx11::error) << "Unknown or unsupported attribute data type";
        }
    }

    static mge::data_type 
    data_type_of_variable(const D3D11_SHADER_TYPE_DESC& typedesc)
    {
        if(typedesc.Class == D3D_SVC_VECTOR) {
            if(typedesc.Type == D3D_SVT_FLOAT) {
                // dimension of vector is in "Columns"
                if(typedesc.Columns == 2) {
                    return mge::data_type::FLOAT_VEC2;
                } else if(typedesc.Columns == 3) {
                    return mge::data_type::FLOAT_VEC3;
                } else if(typedesc.Columns == 4) {
                    return mge::data_type::FLOAT_VEC4;
                }
            }
        }

        MGE_THROW(dx11::error) << "Unsupported shader variable type";
    }

    void
    pipeline::reflect_vertex_shader(dx11::shader_program *s)
    {
        ID3D11ShaderReflection *shader_reflection = nullptr;
        D3DReflect(s->code()->GetBufferPointer(), 
                   s->code()->GetBufferSize(),
                   IID_ID3D11ShaderReflection,
                   (void **)&shader_reflection);
        if(shader_reflection) {
            D3D11_SHADER_DESC shader_desc = {};
            shader_reflection->GetDesc(&shader_desc);
            // dump_shader_desc(shader_desc);
            
            for (uint32_t i=0; i<shader_desc.InputParameters; ++i) {
                D3D11_SIGNATURE_PARAMETER_DESC parameter_desc = {};
                shader_reflection->GetInputParameterDesc(i, &parameter_desc);
                attribute attr;
                attr.name = parameter_desc.SemanticName;
                attr.type = data_type_of_parameter(parameter_desc);
                attr.size = 1;
                m_attributes.push_back(attr);
            }

            for (uint32_t i=0; i<shader_desc.ConstantBuffers; ++i) {
                ID3D11ShaderReflectionConstantBuffer *cbuffer = 
                    shader_reflection->GetConstantBufferByIndex(i);
                D3D11_SHADER_BUFFER_DESC cbuffer_desc = {};
                cbuffer->GetDesc(&cbuffer_desc);
                if (strcmp(cbuffer_desc.Name, "$Globals") == 0) {
                    for (uint32_t j = 0; j<cbuffer_desc.Variables; ++j) {
                        ID3D11ShaderReflectionVariable *var = 
                            cbuffer->GetVariableByIndex(j);
                        D3D11_SHADER_VARIABLE_DESC var_desc = {};
                        var->GetDesc(&var_desc);
                        ID3D11ShaderReflectionType *var_type = var->GetType();
                        D3D11_SHADER_TYPE_DESC type_desc = {};
                        var_type->GetDesc(&type_desc);
                        uniform u;
                        u.name = var_desc.Name;
                        u.type = data_type_of_variable(type_desc);
                        u.size = 1;
                        m_uniforms.push_back(u);
                    }
                } else {
                    MGE_THROW(dx11::error) << "Constant buffers not yet supported";
                }
            }

            shader_reflection->Release();
        }
    }

    void
    pipeline::on_link()
    {
        if (!m_shaders[(size_t)mge::shader_type::VERTEX]
            || !m_shaders[(size_t)mge::shader_type::FRAGMENT]) {
            MGE_THROW(mge::illegal_state) << "Cannot link empty pipeline";
        }

        dx11::shader_program *vs = static_cast<dx11::shader_program *>(m_shaders[(size_t)mge::shader_type::VERTEX].get());
        reflect_vertex_shader(vs);
        m_vertex_shader_uniform_count = m_uniforms.size();
        //dx11::shader *ps = static_cast<dx11::shader *>(m_shaders[(size_t)mge::shader_type::FRAGMENT].get());
        //reflect_shader(ps);
    }

    void
    pipeline::on_set_shader_program(const mge::shader_program_ref& s)
    {
        m_shaders[(size_t)s->type()] = s;
    }
}

#include "pipeline.hpp"
#include "shader.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(DX11);

namespace dx11 {
    pipeline::pipeline(mge::render_context& context)
        : mge::pipeline(context)
    {}

    pipeline::~pipeline()
    {}

    static void
    dump_shader_desc(const D3D10_SHADER_DESC& desc)
    {
        MGE_DEBUG_LOG(DX11) << "Version: " << desc.Version;
        MGE_DEBUG_LOG(DX11) << "Creator: " << desc.Creator;
        MGE_DEBUG_LOG(DX11) << "Flags  : " << desc.Flags;

        MGE_DEBUG_LOG(DX11) << "Constant Buffers  : " << desc.ConstantBuffers;
        MGE_DEBUG_LOG(DX11) << "Bound Resources   : " << desc.BoundResources;
        MGE_DEBUG_LOG(DX11) << "Input Parameters  : " << desc.InputParameters;
        MGE_DEBUG_LOG(DX11) << "Output Parameters : " << desc.OutputParameters;
    }

    void
    pipeline::reflect_shader(dx11::shader *s)
    {
        ID3D10ShaderReflection *shader_reflection = nullptr;
        D3DReflect(s->code()->GetBufferPointer(), 
                   s->code()->GetBufferSize(),
                   IID_ID3D11ShaderReflection,
                   (void **)&shader_reflection);
        // if(shader_reflection) {
        //     D3D10_SHADER_DESC shader_desc = {};
        //     shader_reflection->GetDesc(&shader_desc);
        //     dump_shader_desc(shader_desc);

        //     for (uint32_t i=0; i<shader_desc.InputParameters; ++i) {
        //         D3D10_SIGNATURE_PARAMETER_DESC parameter_desc = {};
        //         shader_reflection->GetInputParameterDesc(i, &parameter_desc);
        //         MGE_DEBUG_LOG(DX11) << "Parameter " << i << " Semantic Name: " << parameter_desc.SemanticName;
        //     }

        //     shader_reflection->Release();
        // }
    }

    void
    pipeline::on_link()
    {
        if (!m_shaders[(size_t)mge::shader_type::VERTEX]
            || !m_shaders[(size_t)mge::shader_type::FRAGMENT]) {
            MGE_THROW(mge::illegal_state) << "Cannot link empty pipeline";
        }

        dx11::shader *vs = static_cast<dx11::shader *>(m_shaders[(size_t)mge::shader_type::VERTEX].get());
        reflect_shader(vs);
        //dx11::shader *ps = static_cast<dx11::shader *>(m_shaders[(size_t)mge::shader_type::FRAGMENT].get());
        //reflect_shader(ps);
    }

    void
    pipeline::on_set_shader(const mge::shader_ref& shader)
    {
        m_shaders[(size_t)shader->type()] = shader;
    }
}
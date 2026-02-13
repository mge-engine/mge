// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "error.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/win32/com_scope.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {

    program::program(render_context& context)
        : ::mge::program(context)
    {}

    program::~program()
    {
        auto dx12_context = static_cast<dx12::render_context*>(&context());
        dx12_context->remove_pipeline_state(this);
    }

    void program::on_link()
    {
        if (!program_shader(mge::shader_type::VERTEX) ||
            !program_shader(mge::shader_type::FRAGMENT)) {
            MGE_THROW(mge::illegal_state) << "Cannot link empty program";
        }
        collect_information();
        create_root_signature();
    }

    void program::on_set_shader(mge::shader* s)
    {
        auto* dx12_shader = static_cast<shader*>(s);
        auto  index = mge::to_underlying(s->type());
        m_shaders[index] = dx12_shader;
    }

    void program::collect_information()
    {
        for (auto& buffers : m_shader_buffers) {
            buffers.clear();
        }

        for (const auto& s : m_shaders) {
            if (s) {
                const dx12::shader* dx12_s =
                    static_cast<const dx12::shader*>(s);
                dx12_s->reflect(m_attributes,
                                m_uniforms,
                                m_uniform_block_metadata);

                auto index = mge::to_underlying(s->type());
                for (const auto& ub : m_uniform_block_metadata) {
                    m_shader_buffers[index].insert(ub.name);
                }
            }
        }
    }

    void program::create_root_signature()
    {
        std::vector<D3D12_ROOT_PARAMETER> root_parameters;

        for (const auto& ub : m_uniform_block_metadata) {
            D3D12_ROOT_PARAMETER param = {};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
            param.Descriptor.ShaderRegister = ub.location;
            param.Descriptor.RegisterSpace = 0;

            if (uses_in_vertex_shader(ub.name) &&
                uses_in_pixel_shader(ub.name)) {
                param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            } else if (uses_in_vertex_shader(ub.name)) {
                param.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
            } else if (uses_in_pixel_shader(ub.name)) {
                param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
            } else {
                param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            }

            root_parameters.push_back(param);
        }

        // Descriptor table for texture SRV (t0)
        D3D12_DESCRIPTOR_RANGE srv_range = {};
        srv_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        srv_range.NumDescriptors = 1;
        srv_range.BaseShaderRegister = 0;
        srv_range.RegisterSpace = 0;
        srv_range.OffsetInDescriptorsFromTableStart =
            D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        D3D12_ROOT_PARAMETER texture_param = {};
        texture_param.ParameterType =
            D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        texture_param.DescriptorTable.NumDescriptorRanges = 1;
        texture_param.DescriptorTable.pDescriptorRanges = &srv_range;
        texture_param.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
        root_parameters.push_back(texture_param);

        // Static sampler for s0
        D3D12_STATIC_SAMPLER_DESC static_sampler = {};
        static_sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        static_sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        static_sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        static_sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        static_sampler.MipLODBias = 0;
        static_sampler.MaxAnisotropy = 0;
        static_sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
        static_sampler.BorderColor =
            D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
        static_sampler.MinLOD = 0.0f;
        static_sampler.MaxLOD = D3D12_FLOAT32_MAX;
        static_sampler.ShaderRegister = 0;
        static_sampler.RegisterSpace = 0;
        static_sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        D3D12_ROOT_SIGNATURE_DESC desc = {
            .NumParameters = static_cast<UINT>(root_parameters.size()),
            .pParameters =
                root_parameters.empty() ? nullptr : root_parameters.data(),
            .NumStaticSamplers = 1,
            .pStaticSamplers = &static_sampler,
            .Flags =
                D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
        };
        ID3DBlob* serialized_signature = nullptr;
        ID3DBlob* error_blob = nullptr;

        HRESULT rc = D3D12SerializeRootSignature(&desc,
                                                 D3D_ROOT_SIGNATURE_VERSION_1,
                                                 &serialized_signature,
                                                 &error_blob);
        if (FAILED(rc)) {
            std::string errormessage(
                static_cast<const char*>(error_blob->GetBufferPointer()),
                static_cast<const char*>(error_blob->GetBufferPointer()) +
                    error_blob->GetBufferSize());
            error_blob->Release();
            throw dx12::error().set_info_from_hresult(
                rc,
                __FILE__,
                __LINE__,
                "",
                "D3D12SerializeRootSignature")
                << "(" << errormessage << ")";
        }

        mge::com_scope<ID3DBlob> signature_scope(serialized_signature);
        rc = dx12_context(context()).device()->CreateRootSignature(
            0,
            serialized_signature->GetBufferPointer(),
            serialized_signature->GetBufferSize(),
            IID_PPV_ARGS(&m_root_signature));
        CHECK_HRESULT(rc, ID3D12Device, CreateRootSignature);
    }

    uint32_t program::buffer_bind_point(const std::string& name) const
    {
        for (const auto& ub : m_uniform_block_metadata) {
            if (ub.name == name) {
                return ub.location;
            }
        }
        MGE_WARNING_TRACE(DX12,
                          "Uniform buffer '{}' not found, using slot 0",
                          name);
        return 0;
    }

    bool program::uses_in_vertex_shader(const std::string& name) const
    {
        auto index = mge::to_underlying(mge::shader_type::VERTEX);
        return m_shader_buffers[index].find(name) !=
               m_shader_buffers[index].end();
    }

    bool program::uses_in_pixel_shader(const std::string& name) const
    {
        auto index = mge::to_underlying(mge::shader_type::FRAGMENT);
        return m_shader_buffers[index].find(name) !=
               m_shader_buffers[index].end();
    }

} // namespace mge::dx12
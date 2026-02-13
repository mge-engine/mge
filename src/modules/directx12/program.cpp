// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "error.hpp"
#include "mge/core/to_underlying.hpp"
#include "mge/win32/com_scope.hpp"
#include "render_context.hpp"
#include "shader.hpp"

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
        for (const auto& s : m_shaders) {
            if (s) {
                const dx12::shader* dx12_s =
                    static_cast<const dx12::shader*>(s);
                dx12_s->reflect(m_attributes,
                                m_uniforms,
                                m_uniform_block_metadata);
            }
        }
    }

    void program::create_root_signature()
    {
        D3D12_ROOT_SIGNATURE_DESC desc = {
            .NumParameters = 0,
            .pParameters = nullptr,
            .NumStaticSamplers = 0,
            .pStaticSamplers = nullptr,
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

} // namespace mge::dx12
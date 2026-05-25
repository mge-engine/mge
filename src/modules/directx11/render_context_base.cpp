// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context_base.hpp"
#include "error.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "frame_buffer.hpp"
#include "index_buffer.hpp"
#include "mge/core/checked_cast.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_buffer_info.hpp"
#include "mge/graphics/memory_image.hpp"
#include "mge/graphics/uniform_block.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    static inline D3D11_BLEND blend_factor_to_dx11(blend_factor factor)
    {
        switch (factor) {
        case blend_factor::ZERO:
            return D3D11_BLEND_ZERO;
        case blend_factor::ONE:
            return D3D11_BLEND_ONE;
        case blend_factor::SRC_COLOR:
            return D3D11_BLEND_SRC_COLOR;
        case blend_factor::ONE_MINUS_SRC_COLOR:
            return D3D11_BLEND_INV_SRC_COLOR;
        case blend_factor::DST_COLOR:
            return D3D11_BLEND_DEST_COLOR;
        case blend_factor::ONE_MINUS_DST_COLOR:
            return D3D11_BLEND_INV_DEST_COLOR;
        case blend_factor::SRC_ALPHA:
            return D3D11_BLEND_SRC_ALPHA;
        case blend_factor::ONE_MINUS_SRC_ALPHA:
            return D3D11_BLEND_INV_SRC_ALPHA;
        case blend_factor::DST_ALPHA:
            return D3D11_BLEND_DEST_ALPHA;
        case blend_factor::ONE_MINUS_DST_ALPHA:
            return D3D11_BLEND_INV_DEST_ALPHA;
        case blend_factor::CONSTANT_COLOR:
            return D3D11_BLEND_BLEND_FACTOR;
        case blend_factor::ONE_MINUS_CONSTANT_COLOR:
            return D3D11_BLEND_INV_BLEND_FACTOR;
        case blend_factor::CONSTANT_ALPHA:
            return D3D11_BLEND_BLEND_FACTOR;
        case blend_factor::ONE_MINUS_CONSTANT_ALPHA:
            return D3D11_BLEND_INV_BLEND_FACTOR;
        case blend_factor::SRC_ALPHA_SATURATE:
            return D3D11_BLEND_SRC_ALPHA_SAT;
        case blend_factor::SRC1_COLOR:
            return D3D11_BLEND_SRC1_COLOR;
        case blend_factor::ONE_MINUS_SRC1_COLOR:
            return D3D11_BLEND_INV_SRC1_COLOR;
        case blend_factor::SRC1_ALPHA:
            return D3D11_BLEND_SRC1_ALPHA;
        case blend_factor::ONE_MINUS_SRC1_ALPHA:
            return D3D11_BLEND_INV_SRC1_ALPHA;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend factor: " << factor;
        }
    }

    static inline D3D11_BLEND_OP blend_operation_to_dx11(blend_operation op)
    {
        switch (op) {
        case blend_operation::NONE:
            return D3D11_BLEND_OP_ADD;
        case blend_operation::ADD:
            return D3D11_BLEND_OP_ADD;
        case blend_operation::SUBTRACT:
            return D3D11_BLEND_OP_SUBTRACT;
        case blend_operation::REVERSE_SUBTRACT:
            return D3D11_BLEND_OP_REV_SUBTRACT;
        case blend_operation::MIN:
            return D3D11_BLEND_OP_MIN;
        case blend_operation::MAX:
            return D3D11_BLEND_OP_MAX;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown blend operation: " << op;
        }
    }

    static inline D3D11_COMPARISON_FUNC depth_test_to_dx11(mge::test func)
    {
        switch (func) {
        case mge::test::NEVER:
            return D3D11_COMPARISON_NEVER;
        case mge::test::LESS:
            return D3D11_COMPARISON_LESS;
        case mge::test::EQUAL:
            return D3D11_COMPARISON_EQUAL;
        case mge::test::LESS_EQUAL:
            return D3D11_COMPARISON_LESS_EQUAL;
        case mge::test::GREATER:
            return D3D11_COMPARISON_GREATER;
        case mge::test::NOT_EQUAL:
            return D3D11_COMPARISON_NOT_EQUAL;
        case mge::test::GREATER_EQUAL:
            return D3D11_COMPARISON_GREATER_EQUAL;
        case mge::test::ALWAYS:
            return D3D11_COMPARISON_ALWAYS;
        default:
            MGE_THROW(mge::illegal_argument)
                << "Unknown depth test: " << static_cast<int>(func);
        }
    }

    render_context_base::render_context_base(mge::dx11::render_system& system,
                                             const mge::extent&        ext)
        : mge::render_context(system, ext)
        , m_render_system(system)
    {}

    render_context_base::~render_context_base()
    {
        auto fd = m_render_system.frame_debugger();
        if (fd) {
            MGE_INFO_TRACE(DX11, "Ending frame recording");
            fd->end_capture();
        }
    }

    void render_context_base::create_default_depth_stencil_state()
    {
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable              = true;
        desc.DepthWriteMask           = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc                = D3D11_COMPARISON_LESS;
        desc.StencilEnable            = true;
        desc.StencilReadMask          = 0xFF;
        desc.StencilWriteMask         = 0xFF;

        desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

        desc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        desc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

        ID3D11DepthStencilState* tmp = nullptr;
        HRESULT rc = m_device->CreateDepthStencilState(&desc, &tmp);
        CHECK_HRESULT(rc, ID3D11Device, CreateDepthStencilState);
        m_depth_stencil_state.reset(tmp);
    }

    void render_context_base::init_capabilities()
    {
        class capabilities : public mge::render_context::capabilities
        {
        public:
            capabilities()  = default;
            ~capabilities() = default;

            uint32_t max_texture_size() const override
            {
                return D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
            }

            uint32_t max_texture_3d_size() const override
            {
                return D3D11_REQ_TEXTURE3D_U_V_OR_W_DIMENSION;
            }

            uint32_t max_texture_cube_size() const override
            {
                return D3D11_REQ_TEXTURECUBE_DIMENSION;
            }

            uint32_t max_texture_array_layers() const override
            {
                return D3D11_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION;
            }

            uint32_t max_vertex_attributes() const override
            {
                return D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT;
            }

            uint32_t max_uniform_buffer_bindings() const override
            {
                return D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT;
            }

            uint32_t max_texture_bindings() const override
            {
                return D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT;
            }

            uint32_t max_color_attachments() const override
            {
                return D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT;
            }
        };

        m_capabilities = std::make_unique<capabilities>();
    }

    mge::index_buffer*
    render_context_base::on_create_index_buffer(mge::data_type dt,
                                                size_t         data_size)
    {
        return new index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer*
    render_context_base::on_create_vertex_buffer(const mge::vertex_layout& layout,
                                                 size_t                    data_size)
    {
        return new mge::dx11::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context_base::on_create_shader(mge::shader_type t)
    {
        return new shader(*this, t);
    }

    mge::program* render_context_base::on_create_program()
    {
        return new program(*this);
    }

    mge::frame_buffer* render_context_base::on_create_frame_buffer(
        const mge::frame_buffer_info& info)
    {
        auto fb = std::make_unique<dx11::frame_buffer>(*this);

        const auto num_color =
            mge::checked_cast<uint32_t>(info.color_attachments.size());
        for (uint32_t i = 0; i < num_color; ++i) {
            const auto& ca  = info.color_attachments[i];
            auto        tex = create_render_target_texture(
                mge::texture_type::TYPE_2D, ca.format, ca.extent);
            fb->attach_color(tex, i);
        }

        if (info.depth_stencil_extent) {
            mge::image_format depth_fmt(
                mge::image_format::data_format::DEPTH_STENCIL,
                mge::data_type::UINT32);
            auto tex =
                create_render_target_texture(mge::texture_type::TYPE_2D,
                                             depth_fmt,
                                             *info.depth_stencil_extent);
            fb->attach_depth(tex);
        }

        return fb.release();
    }

    mge::texture_ref
    render_context_base::create_texture(mge::texture_type type)
    {
        return std::make_shared<texture>(*this, type);
    }

    mge::texture_ref
    render_context_base::create_render_target_texture(
        mge::texture_type        type,
        const mge::image_format& format,
        const mge::extent&       extent)
    {
        return std::make_shared<texture>(*this, type, format, extent);
    }

    void render_context_base::render(const mge::pass& p)
    {
        ID3D11RenderTargetView* rtv = nullptr;
        ID3D11DepthStencilView* dsv = nullptr;

        if (p.frame_buffer()) {
            auto* fb = static_cast<dx11::frame_buffer*>(p.frame_buffer().get());
            rtv      = fb->render_target_view(0);
            dsv      = fb->depth_stencil_view();
        } else {
            rtv = m_render_target_view.get();
            dsv = m_depth_stencil_view.get();
        }
        m_device_context->OMSetRenderTargets(1, &rtv, dsv);

        const auto&    vp = p.viewport();
        D3D11_VIEWPORT dx11_vp =
            {vp.x, vp.y, vp.width, vp.height, vp.min_depth, vp.max_depth};
        m_device_context->RSSetViewports(1, &dx11_vp);

        const auto& sc = p.scissor();
        D3D11_RECT  scissor_rect = {.left   = static_cast<LONG>(sc.left),
                                    .top    = static_cast<LONG>(sc.top),
                                    .right  = static_cast<LONG>(sc.right),
                                    .bottom = static_cast<LONG>(sc.bottom)};
        m_device_context->RSSetScissorRects(1, &scissor_rect);

        if (p.clear_color_enabled() && rtv) {
            float clearcolor[4] = {p.clear_color_value().r,
                                   p.clear_color_value().g,
                                   p.clear_color_value().b,
                                   p.clear_color_value().a};
            m_device_context->ClearRenderTargetView(rtv, clearcolor);
        }

        if (p.clear_depth_enabled() && dsv) {
            m_device_context->ClearDepthStencilView(
                dsv, D3D11_CLEAR_DEPTH, p.clear_depth_value(), 0);
        }

        bool           blend_pass_needed = false;
        mge::rectangle current_scissor   = p.scissor();

        for_each_draw_in_pass(
            p.index(),
            [this,
             &blend_pass_needed,
             &current_scissor,
             &p](program_handle                   prog,
                 vertex_buffer_handle             vertices,
                 index_buffer_handle              indices,
                 const mge::pipeline_state&       state,
                 mge::uniform_block*              ub,
                 const mge::texture_binding_list& textures,
                 uint32_t                         index_count,
                 uint32_t                         index_offset,
                 const mge::rectangle&            cmd_scissor) {
                blend_operation op = state.color_blend_operation();
                if (op == blend_operation::NONE) {
                    const auto& effective =
                        cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                    if (effective != current_scissor) {
                        D3D11_RECT sr = {
                            .left   = static_cast<LONG>(effective.left),
                            .top    = static_cast<LONG>(effective.top),
                            .right  = static_cast<LONG>(effective.right),
                            .bottom = static_cast<LONG>(effective.bottom)};
                        m_device_context->RSSetScissorRects(1, &sr);
                        current_scissor = effective;
                    }
                    ID3D11RasterizerState* rs_state = this->rasterizer_state(state);
                    m_device_context->RSSetState(rs_state);
                    if (!state.depth_write()) {
                        ID3D11DepthStencilState* ds_state =
                            this->depth_stencil_state(state);
                        m_device_context->OMSetDepthStencilState(ds_state, 1);
                    }
                    draw_geometry(prog.get(),
                                  vertices.get(),
                                  indices.get(),
                                  ub,
                                  textures,
                                  index_count,
                                  index_offset);
                    if (!state.depth_write()) {
                        m_device_context->OMSetDepthStencilState(
                            m_depth_stencil_state.get(), 1);
                    }
                } else {
                    blend_pass_needed = true;
                }
            });

        if (blend_pass_needed) {
            for_each_draw_in_pass(
                p.index(),
                [this,
                 &current_scissor,
                 &p](program_handle                   prog,
                     vertex_buffer_handle             vertices,
                     index_buffer_handle              indices,
                     const mge::pipeline_state&       state,
                     mge::uniform_block*              ub,
                     const mge::texture_binding_list& textures,
                     uint32_t                         index_count,
                     uint32_t                         index_offset,
                     const mge::rectangle&            cmd_scissor) {
                    blend_operation op = state.color_blend_operation();
                    if (op != blend_operation::NONE) {
                        const auto& effective =
                            cmd_scissor.area() != 0 ? cmd_scissor : p.scissor();
                        if (effective != current_scissor) {
                            D3D11_RECT sr = {
                                .left   = static_cast<LONG>(effective.left),
                                .top    = static_cast<LONG>(effective.top),
                                .right  = static_cast<LONG>(effective.right),
                                .bottom = static_cast<LONG>(effective.bottom)};
                            m_device_context->RSSetScissorRects(1, &sr);
                            current_scissor = effective;
                        }
                        ID3D11RasterizerState* rs_state =
                            this->rasterizer_state(state);
                        m_device_context->RSSetState(rs_state);

                        ID3D11BlendState* blend_state_obj =
                            this->blend_state(state);
                        float blend_factor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                        m_device_context->OMSetBlendState(blend_state_obj,
                                                          blend_factor,
                                                          0xffffffff);

                        if (!state.depth_write()) {
                            ID3D11DepthStencilState* ds_state =
                                this->depth_stencil_state(state);
                            m_device_context->OMSetDepthStencilState(ds_state,
                                                                      1);
                        }
                        draw_geometry(prog.get(),
                                      vertices.get(),
                                      indices.get(),
                                      ub,
                                      textures,
                                      index_count,
                                      index_offset);
                        if (!state.depth_write()) {
                            m_device_context->OMSetDepthStencilState(
                                m_depth_stencil_state.get(), 1);
                        }
                    }
                });
            m_device_context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
        }
    }

    void render_context_base::draw_geometry(
        mge::program*                    prog,
        mge::vertex_buffer*              vb,
        mge::index_buffer*               ib,
        mge::uniform_block*              ub,
        const mge::texture_binding_list& textures,
        uint32_t                         index_count,
        uint32_t                         index_offset)
    {
        if (!prog) {
            MGE_THROW(illegal_state) << "Draw command has no program assigned";
        }
        dx11::program& dx11_prog = static_cast<dx11::program&>(*prog);

        if (ub) {
            bind_uniform_block(dx11_prog, *ub);
        }

        const dx11::shader* dx11_vertex_shader =
            static_cast<const dx11::shader*>(
                dx11_prog.program_shader(mge::shader_type::VERTEX));

        if (!vb) {
            MGE_THROW(illegal_state)
                << "Draw command has no vertex buffer assigned";
        }

        ID3D11InputLayout* input_layout =
            get_or_create_input_layout(vb->layout(),
                                       dx11_vertex_shader->code());
        m_device_context->IASetInputLayout(input_layout);
        m_device_context->IASetPrimitiveTopology(
            D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        const dx11::vertex_buffer* dx11_vb =
            static_cast<const dx11::vertex_buffer*>(vb);
        UINT          element_size   = static_cast<UINT>(dx11_vb->layout().binary_size());
        UINT          dummy_offset   = 0;
        ID3D11Buffer* vb_buffer      = dx11_vb->buffer();
        m_device_context->IASetVertexBuffers(0,
                                             1,
                                             &vb_buffer,
                                             &element_size,
                                             &dummy_offset);

        if (!ib) {
            MGE_THROW(illegal_state)
                << "Draw command has no index buffer assigned";
        }
        const dx11::index_buffer* dx11_ib =
            static_cast<const dx11::index_buffer*>(ib);
        ID3D11Buffer* ib_buffer = dx11_ib->buffer();

        DXGI_FORMAT index_format;
        switch (dx11_ib->element_type()) {
        case mge::data_type::UINT16:
            index_format = DXGI_FORMAT_R16_UINT;
            break;
        case mge::data_type::INT32:
        case mge::data_type::UINT32:
            index_format = DXGI_FORMAT_R32_UINT;
            break;
        default:
            MGE_THROW(mge::illegal_state)
                << "Unsupported index buffer type: "
                << static_cast<int>(dx11_ib->element_type());
        }
        m_device_context->IASetIndexBuffer(ib_buffer, index_format, 0);

        m_device_context->VSSetShader(
            dx11_vertex_shader->directx_vertex_shader(), nullptr, 0);

        const dx11::shader* dx11_pixel_shader =
            static_cast<const dx11::shader*>(
                dx11_prog.program_shader(mge::shader_type::FRAGMENT));
        m_device_context->PSSetShader(
            dx11_pixel_shader->directx_pixel_shader(), nullptr, 0);

        for (const auto& binding : textures) {
            if (binding.texture) {
                dx11::texture& dx11_tex =
                    static_cast<dx11::texture&>(*binding.texture);
                ID3D11ShaderResourceView* srv = dx11_tex.shader_resource_view();
                m_device_context->PSSetShaderResources(binding.slot, 1, &srv);
                ID3D11SamplerState* sampler = dx11_tex.sampler_state();
                m_device_context->PSSetSamplers(binding.slot, 1, &sampler);
            }
        }

        UINT element_count =
            index_count > 0
                ? index_count
                : static_cast<UINT>(dx11_ib->element_count());
        m_device_context->DrawIndexed(element_count, index_offset, 0);

        for (const auto& binding : textures) {
            if (binding.texture) {
                ID3D11ShaderResourceView* null_srv = nullptr;
                m_device_context->PSSetShaderResources(binding.slot,
                                                       1,
                                                       &null_srv);
            }
        }
    }

    ID3D11BlendState*
    render_context_base::blend_state(const mge::pipeline_state& state)
    {
        auto it = m_blend_state_cache.find(state);
        if (it != m_blend_state_cache.end()) {
            return it->second.get();
        }

        blend_operation color_op  = state.color_blend_operation();
        blend_factor    color_src = state.color_blend_factor_src();
        blend_factor    color_dst = state.color_blend_factor_dst();
        blend_operation alpha_op  = state.alpha_blend_operation();
        blend_factor    alpha_src = state.alpha_blend_factor_src();
        blend_factor    alpha_dst = state.alpha_blend_factor_dst();

        D3D11_BLEND_DESC blend_desc                    = {};
        blend_desc.RenderTarget[0].BlendEnable         = TRUE;
        blend_desc.RenderTarget[0].SrcBlend            = blend_factor_to_dx11(color_src);
        blend_desc.RenderTarget[0].DestBlend           = blend_factor_to_dx11(color_dst);
        blend_desc.RenderTarget[0].BlendOp             = blend_operation_to_dx11(color_op);
        blend_desc.RenderTarget[0].SrcBlendAlpha       = blend_factor_to_dx11(alpha_src);
        blend_desc.RenderTarget[0].DestBlendAlpha      = blend_factor_to_dx11(alpha_dst);
        blend_desc.RenderTarget[0].BlendOpAlpha        = blend_operation_to_dx11(alpha_op);
        blend_desc.RenderTarget[0].RenderTargetWriteMask =
            D3D11_COLOR_WRITE_ENABLE_ALL;

        ID3D11BlendState* blend_state_obj = nullptr;
        HRESULT rc = m_device->CreateBlendState(&blend_desc, &blend_state_obj);
        CHECK_HRESULT(rc, ID3D11Device, CreateBlendState);

        com_unique_ptr<ID3D11BlendState> owned_ptr(blend_state_obj);
        m_blend_state_cache[state] = std::move(owned_ptr);
        return m_blend_state_cache[state].get();
    }

    ID3D11DepthStencilState*
    render_context_base::depth_stencil_state(const mge::pipeline_state& state)
    {
        auto it = m_depth_stencil_state_cache.find(state);
        if (it != m_depth_stencil_state_cache.end()) {
            return it->second.get();
        }

        D3D11_DEPTH_STENCIL_DESC desc        = {};
        desc.DepthEnable                     = TRUE;
        desc.DepthWriteMask                  = state.depth_write()
                                                   ? D3D11_DEPTH_WRITE_MASK_ALL
                                                   : D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = depth_test_to_dx11(state.depth_test_function());
        desc.StencilEnable                   = TRUE;
        desc.StencilReadMask                 = 0xFF;
        desc.StencilWriteMask                = 0xFF;
        desc.FrontFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilDepthFailOp    = D3D11_STENCIL_OP_INCR;
        desc.FrontFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;
        desc.BackFace.StencilFailOp          = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilDepthFailOp     = D3D11_STENCIL_OP_DECR;
        desc.BackFace.StencilPassOp          = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc            = D3D11_COMPARISON_ALWAYS;

        ID3D11DepthStencilState* ds_obj = nullptr;
        HRESULT rc = m_device->CreateDepthStencilState(&desc, &ds_obj);
        CHECK_HRESULT(rc, ID3D11Device, CreateDepthStencilState);

        com_unique_ptr<ID3D11DepthStencilState> owned_ptr(ds_obj);
        m_depth_stencil_state_cache[state] = std::move(owned_ptr);
        return m_depth_stencil_state_cache[state].get();
    }

    void render_context_base::bind_uniform_block(mge::dx11::program& dx11_program,
                                                 mge::uniform_block& ub)
    {
        ub.sync_from_globals();

        ID3D11Buffer* cbuffer = nullptr;
        auto          it      = m_constant_buffers.find(&ub);
        if (it != m_constant_buffers.end()) {
            cbuffer = it->second.get();
        } else {
            D3D11_BUFFER_DESC cbuffer_desc  = {};
            cbuffer_desc.ByteWidth          = static_cast<UINT>(ub.data_size());
            cbuffer_desc.Usage              = D3D11_USAGE_DYNAMIC;
            cbuffer_desc.BindFlags          = D3D11_BIND_CONSTANT_BUFFER;
            cbuffer_desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;

            HRESULT rc = m_device->CreateBuffer(&cbuffer_desc, nullptr, &cbuffer);
            CHECK_HRESULT(rc, ID3D11Device, CreateBuffer);

            m_constant_buffers[&ub]         = mge::make_com_unique_ptr(cbuffer);
            m_constant_buffer_versions[&ub] = 0;
        }

        auto& cached_version = m_constant_buffer_versions[&ub];
        if (cached_version != ub.version()) {
            D3D11_MAPPED_SUBRESOURCE mapped_resource;
            HRESULT rc = m_device_context->Map(cbuffer,
                                               0,
                                               D3D11_MAP_WRITE_DISCARD,
                                               0,
                                               &mapped_resource);
            CHECK_HRESULT(rc, ID3D11DeviceContext, Map);

            memcpy(mapped_resource.pData, ub.data(), ub.data_size());
            m_device_context->Unmap(cbuffer, 0);

            cached_version = ub.version();
        }

        uint32_t bind_point = dx11_program.buffer_bind_point(ub.name());

        if (dx11_program.uses_in_vertex_shader(ub.name())) {
            m_device_context->VSSetConstantBuffers(bind_point, 1, &cbuffer);
        }
        if (dx11_program.uses_in_pixel_shader(ub.name())) {
            m_device_context->PSSetConstantBuffers(bind_point, 1, &cbuffer);
        }
    }

    ID3D11RasterizerState*
    render_context_base::rasterizer_state(const mge::pipeline_state& state)
    {
        auto it = m_rasterizer_state_cache.find(state);
        if (it != m_rasterizer_state_cache.end()) {
            return it->second.get();
        }

        D3D11_RASTERIZER_DESC rasterizer_desc    = {};
        rasterizer_desc.FillMode                 = D3D11_FILL_SOLID;
        rasterizer_desc.FrontCounterClockwise    = FALSE;
        rasterizer_desc.DepthBias               = 0;
        rasterizer_desc.DepthBiasClamp          = 0.0f;
        rasterizer_desc.SlopeScaledDepthBias    = 0.0f;
        rasterizer_desc.DepthClipEnable         = TRUE;
        rasterizer_desc.ScissorEnable           = TRUE;
        rasterizer_desc.MultisampleEnable       = FALSE;
        rasterizer_desc.AntialiasedLineEnable   = FALSE;

        mge::cull_mode mode = state.cull_mode();
        switch (mode) {
        case mge::cull_mode::NONE:
            rasterizer_desc.CullMode = D3D11_CULL_NONE;
            break;
        case mge::cull_mode::CLOCKWISE:
            rasterizer_desc.CullMode = D3D11_CULL_FRONT;
            break;
        case mge::cull_mode::COUNTER_CLOCKWISE:
            rasterizer_desc.CullMode = D3D11_CULL_BACK;
            break;
        }

        ID3D11RasterizerState* rs_obj = nullptr;
        HRESULT rc =
            m_device->CreateRasterizerState(&rasterizer_desc, &rs_obj);
        CHECK_HRESULT(rc, ID3D11Device, CreateRasterizerState);

        com_unique_ptr<ID3D11RasterizerState> owned_ptr(rs_obj);
        m_rasterizer_state_cache[state] = std::move(owned_ptr);
        return m_rasterizer_state_cache[state].get();
    }

    ID3D11InputLayout*
    render_context_base::get_or_create_input_layout(
        const mge::vertex_layout& layout, ID3DBlob* shader_code)
    {
        for (auto& entry : m_cached_input_layouts) {
            if (entry.layout == layout && entry.shader_blob == shader_code) {
                return entry.input_layout.get();
            }
        }

        D3D11_INPUT_ELEMENT_DESC* descs = m_input_layout_cache.get(layout);

        ID3D11InputLayout* input_layout = nullptr;
        HRESULT            rc =
            m_device->CreateInputLayout(descs,
                                        static_cast<UINT>(layout.size()),
                                        shader_code->GetBufferPointer(),
                                        shader_code->GetBufferSize(),
                                        &input_layout);
        CHECK_HRESULT(rc, ID3D11Device, CreateInputLayout);

        m_cached_input_layouts.push_back(
            {layout, shader_code, mge::make_com_unique_ptr(input_layout)});
        return input_layout;
    }

} // namespace mge::dx11

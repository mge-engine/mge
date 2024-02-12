// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "memory_command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/overloaded.hpp"
#include "mge/core/trace.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "topology.hpp"
#include "vertex_buffer.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    memory_command_list::memory_command_list(render_context& context)
        : mge::command_list(context, false)
        , m_dx11_context(context)
    {}

    memory_command_list::~memory_command_list() {}

    void memory_command_list::clear(const rgba_color& c)
    {
        m_commands.push_back(clear_command{c});
    }

    void memory_command_list::draw(const mge::draw_command& command)
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

        const dx11::program* dx11_program =
            static_cast<const dx11 ::program*>(command.program().get());

        const dx11::shader* dx11_vertex_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::VERTEX).get());

        ID3D11InputLayout* input_layout = nullptr;

        auto rc = m_dx11_context.device()->CreateInputLayout(
            input_elements,
            1,
            dx11_vertex_shader->code()->GetBufferPointer(),
            dx11_vertex_shader->code()->GetBufferSize(),
            &input_layout);
        CHECK_HRESULT(rc, ID3D11Device, CreateInputLayout);

        // m_commands.push_back(draw_command{command, input_layout});

        MGE_DEBUG_TRACE(DX11) << "Draw call";
    }

    void memory_command_list::execute()
    {
        for (auto& cmd : m_commands) {
            std::visit(
                [&](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, clear_command>) {
                        float clear_color[4] = {arg.clear_color.r,
                                                arg.clear_color.g,
                                                arg.clear_color.b,
                                                arg.clear_color.a};
                        m_dx11_context.device_context()->ClearRenderTargetView(
                            m_dx11_context.render_target_view(),
                            clear_color);
                    } else if constexpr (std::is_same_v<T, draw_command>) {
                        perform_drawing(arg);
                    }
                },
                cmd);
        }
    }

    void memory_command_list::perform_drawing(
        const mge::dx11::memory_command_list::draw_command& cmd)
    {
#if 0
        auto device_context = m_dx11_context.device_context();

        auto input_layout = cmd.layout;
        device_context->IASetInputLayout(input_layout);

        const dx11::vertex_buffer* dx11_vertex_buffer =
            static_cast<const dx11::vertex_buffer*>(
                cmd.command-vertices().get());
        UINT element_size =
            static_cast<UINT>(cmd.command.vertices()->layout().binary_size());
        UINT          stride = 0;
        ID3D11Buffer* vertex_buffer = dx11_vertex_buffer->buffer();
        device_context->IASetVertexBuffers(0,
                                           1,
                                           &vertex_buffer,
                                           &element_size,
                                           &stride);

        const dx11::index_buffer* dx11_index_buffer =
            static_cast<const dx11::index_buffer*>(cmd.command.indices().get());
        ID3D11Buffer* index_buffer = dx11_index_buffer->buffer();
        device_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

        device_context->IASetPrimitiveTopology(
            to_dx11_topology(cmd.command.topology()));

        const dx11::program* dx11_program =
            static_cast<const dx11 ::program*>(cmd.command.program().get());

        const dx11::shader* dx11_vertex_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::VERTEX).get());

        device_context->VSSetShader(dx11_vertex_shader->directx_vertex_shader(),
                                    nullptr,
                                    0);

        const dx11::shader* dx11_pixel_shader =
            static_cast<const dx11::shader*>(
                dx11_program->program_shader(mge::shader_type::FRAGMENT).get());
        device_context->PSSetShader(dx11_pixel_shader->directx_pixel_shader(),
                                    nullptr,
                                    0);
        UINT element_count =
            static_cast<UINT>(cmd.command.indices()->element_count());
        device_context->DrawIndexed(element_count, 0, 0);
#endif
    }

} // namespace mge::dx11
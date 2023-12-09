// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "memory_command_list.hpp"
#include "error.hpp"
#include "mge/core/overloaded.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "vertex_buffer.hpp"

namespace mge::dx11 {

    struct semantic_context
    {
        semantic_context()
            : position_used(false)
        {}

        bool position_used;
    };

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
        m_commands.push_back(draw_command{command});

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

        m_layouts[&command] = input_layout;
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
                        perform_drawing(arg.command.get());
                    }
                },
                cmd);
        }
    }

    void memory_command_list::perform_drawing(const mge::draw_command& command)
    {}

} // namespace mge::dx11
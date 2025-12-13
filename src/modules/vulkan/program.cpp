// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "program.hpp"
#include "render_context.hpp"
#include "shader.hpp"

namespace mge::vulkan {
    program::program(render_context& context)
        : mge::program(context)
    {}

    program::~program() {}

    void program::on_link()
    {
        m_shader_stage_create_infos.clear();
        for (const auto& shader : m_shaders) {
            m_shader_stage_create_infos.push_back(
                shader->pipeline_stage_info());
        }
    }

    void program::on_set_shader(mge::shader* s)
    {
        auto vulkan_shader = static_cast<mge::vulkan::shader*>(s);
        m_shaders.push_back(vulkan_shader);
    }
} // namespace mge::vulkan
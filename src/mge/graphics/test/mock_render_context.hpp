// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {
    class mock_render_context : public render_context
    {
    public:

        mock_render_context(async_executor *display_executor)
            :render_context(display_executor)
        {}

        MOCK_METHOD0(flush,
                     void());
        MOCK_METHOD4(create_index_buffer,
                     index_buffer_ref (data_type,
                                       mge::usage,
                                       size_t,
                                       void*));

        MOCK_METHOD4(create_vertex_buffer,
                     mge::vertex_buffer_ref (const mge::vertex_layout&,
                                             mge::usage,
                                             size_t,
                                             void *));
        MOCK_METHOD1(create_texture_2d,
                     mge::texture_2d_ref(const mge::image_ref&));
        MOCK_METHOD0(create_texture_2d,
                     mge::texture_2d_ref());

        MOCK_METHOD1(create_shader_program, shader_program_ref(shader_type));
        MOCK_METHOD0(create_pipeline, pipeline_ref());
        MOCK_METHOD0(create_command_list, command_list_ref());
        MOCK_CONST_METHOD1(shader_languages,
                           void (std::vector<shader_language>&));
        MOCK_METHOD1(execute, void(const mge::command_list_ref&));

        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(usage usage,
                                             const std::array<T, N>& buffer)
        {
            return render_context::create_index_buffer(usage,
                                                       buffer);
        }

    };
}

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
        MOCK_METHOD6(create_index_buffer,
                     index_buffer_ref (data_type,
                                       usage,
                                       buffer_access,
                                       buffer_access,
                                       size_t,
                                       void*));

        MOCK_METHOD6(create_vertex_buffer,
                     mge::vertex_buffer_ref (const mge::vertex_layout&,
                                             mge::usage,
                                             mge::buffer_access,
                                             mge::buffer_access,
                                             size_t,
                                             void *));
        MOCK_METHOD5(create_texture,
                     mge::texture_ref (mge::texture_type,
                                       mge::usage,
                                       mge::filter_function,
                                       mge::filter_function,
                                       bool));
        MOCK_METHOD1(create_shader, shader_ref(shader_type));
        MOCK_METHOD0(create_pipeline, pipeline_ref());
        
        MOCK_CONST_METHOD1(shader_languages,
                           void (std::vector<shader_language>&));

        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(usage usage,
                                             buffer_access cpu_access,
                                             buffer_access gpu_access,
                                             const std::array<T, N>& buffer)
        {
            return render_context::create_index_buffer(usage,
                                                       cpu_access,
                                                       gpu_access,
                                                       buffer);
        }

    };
}

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
                                       buffer_change_policy,
                                       buffer_access,
                                       buffer_access,
                                       size_t,
                                       void*));
        MOCK_METHOD1(create_shader, shader_ref(shader_type));
        MOCK_CONST_METHOD1(shader_languages,
                           void (std::vector<shader_language>&));

        template <typename T, std::size_t N>
        index_buffer_ref create_index_buffer(buffer_change_policy change_policy,
                                             buffer_access cpu_access,
                                             buffer_access gpu_access,
                                             const std::array<T, N>& buffer)
        {
            return render_context::create_index_buffer(change_policy,
                                                       cpu_access,
                                                       gpu_access,
                                                       buffer);
        }

    };
}

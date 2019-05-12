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
        MOCK_METHOD3(create_index_buffer,
                     index_buffer_ref (buffer_usage,
                                       buffer_access,
                                       buffer_access));
        MOCK_METHOD5(create_index_buffer,
                     index_buffer_ref (buffer_usage,
                                       buffer_access,
                                       buffer_access,
                                       void*,
                                       size_t));
        MOCK_METHOD1(create_shader, shader_ref(shader_type));
        MOCK_CONST_METHOD1(shader_languages,
                           void (std::vector<shader_language>&));
    };
}

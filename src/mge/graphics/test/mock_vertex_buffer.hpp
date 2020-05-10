// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/vertex_buffer.hpp"
#include "test/googlemock.hpp"

namespace mge {
    class mock_vertex_buffer : public vertex_buffer
    {
    public:
        mock_vertex_buffer(render_context &context, const vertex_layout &layout,
                           usage usage, size_t element_count,
                           void *initial_data)
            : vertex_buffer(context, layout, usage, element_count, initial_data)
        {}

        ~mock_vertex_buffer() = default;

        MOCK_CONST_METHOD0(size, size_t());
        MOCK_METHOD0(on_map, void *());
        MOCK_METHOD0(on_unmap, void());
    };
} // namespace mge
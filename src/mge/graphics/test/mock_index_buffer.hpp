// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "test/googlemock.hpp"

namespace mge {
    class mock_index_buffer : public index_buffer
    {
    public:
        mock_index_buffer(render_context &context, data_type type, usage usage,
                          size_t element_count, void *initial_data = nullptr)
            : index_buffer(context, type, usage, element_count, initial_data)
        {}

        ~mock_index_buffer() = default;

        MOCK_CONST_METHOD0(size, size_t());
        MOCK_METHOD0(on_map, void *());
        MOCK_METHOD0(on_unmap, void());
    };
} // namespace mge
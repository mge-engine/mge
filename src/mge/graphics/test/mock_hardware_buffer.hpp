// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googlemock.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/hardware_buffer.hpp"

namespace mge {
    class mock_hardware_buffer : public hardware_buffer
    {
    public:
        mock_hardware_buffer(render_context& context,
                             buffer_type type,
                             usage usage)
            :hardware_buffer(context,
                             type,
                             usage)
        {}

        mock_hardware_buffer(render_context& context,
                             buffer_type type,
                             usage usage,
                             void *data,
                             size_t data_size)
            :hardware_buffer(context,
                             type,
                             usage,
                             data,
                             data_size)
        {}

        ~mock_hardware_buffer() = default;
        MOCK_CONST_METHOD0(size, size_t());
        MOCK_METHOD0(on_map, void *());
        MOCK_METHOD0(on_unmap, void());
    };
}

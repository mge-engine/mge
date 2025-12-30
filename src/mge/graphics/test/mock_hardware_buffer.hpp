// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/hardware_buffer.hpp"
#include "test/googlemock.hpp"

class MOCK_hardware_buffer : public mge::hardware_buffer
{
public:
    MOCK_hardware_buffer(mge::render_context& context,
                         mge::buffer_type     type,
                         size_t               data_size)
        : mge::hardware_buffer(context, type, data_size)
    {}

    ~MOCK_hardware_buffer() {}

    MOCK_METHOD(void*, on_map, (), ());
    MOCK_METHOD(void, on_unmap, (), ());
    MOCK_METHOD(void, on_set_data, (void* data, size_t data_size), ());
};

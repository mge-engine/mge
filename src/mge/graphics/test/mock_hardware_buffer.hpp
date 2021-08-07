// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/hardware_buffer.hpp"
#include "test/googlemock.hpp"

class MOCK_hardware_buffer : public mge::hardware_buffer
{
public:
    MOCK_hardware_buffer(mge::render_context& context,
                         mge::buffer_type     type,
                         size_t               data_size,
                         void*                data)
        : mge::hardware_buffer(context, type, data_size, data)
    {}

    ~MOCK_hardware_buffer() {}

    MOCK_METHOD(void*, on_map, (), ());
    MOCK_METHOD(void, on_unmap, (), ());
};

// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/index_buffer.hpp"
#include "test/googlemock.hpp"

class MOCK_index_buffer : public mge::index_buffer
{
public:
    MOCK_index_buffer(mge::render_context& context, mge::buffer_type type)
        : mge::index_buffer(context, type)
    {}

    MOCK_index_buffer(mge::render_context& context,
                      mge::buffer_type     type,
                      void*                data,
                      size_t               data_size)
        : mge::index_buffer(context, type, data, data_size)
    {}

    ~MOCK_hardware_buffer() {}

    MOCK_METHOD(size_t, size, (), (const override));
    MOCK_METHOD(void*, on_map, (), ());
    MOCK_METHOD(void, on_unmap, (), ());
};

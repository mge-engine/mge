// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/input_stream.hpp"
#include "test/googlemock.hpp"

class MOCK_input_stream : public mge::input_stream
{
public:
    MOCK_input_stream() = default;
    ~MOCK_input_stream() = default;

    MOCK_METHOD(streamsize_type, read, (), (override));

    using mge::input_stream::read;

    MOCK_METHOD(offset_type, position, (), (override));
    MOCK_METHOD(offset_type,
                seek,
                (offset_type offset, direction_type dir),
                (override));
    MOCK_METHOD(streamsize_type,
                on_read,
                (void* destination, streamsize_type size),
                ());

    streamsize_type inherit_read()
    {
        return input_stream::read();
    }
};

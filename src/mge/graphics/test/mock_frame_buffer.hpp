// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/frame_buffer.hpp"
#include "test/googlemock.hpp"

class MOCK_frame_buffer : public mge::frame_buffer
{
public:
    explicit MOCK_frame_buffer(mge::render_context& ctx)
        : mge::frame_buffer(ctx)
    {}
    ~MOCK_frame_buffer() {}

    MOCK_METHOD(void, on_attach_color, (const mge::texture_ref&, uint32_t), ());
    MOCK_METHOD(void, on_attach_depth, (const mge::texture_ref&), ());
};

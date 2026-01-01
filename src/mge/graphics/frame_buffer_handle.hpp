// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/handle.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    using frame_buffer_handle =
        mge::handle<mge::frame_buffer, mge::render_context>;

}
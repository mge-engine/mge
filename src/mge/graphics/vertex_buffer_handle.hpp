// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/handle.hpp"
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/graphics_fwd.hpp"

namespace mge {

    using vertex_buffer_handle =
        mge::handle<mge::vertex_buffer, mge::render_context>;

}
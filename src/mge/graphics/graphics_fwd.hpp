// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/memory.hpp"

namespace mge {

    MGE_DECLARE_REF(render_system);
    MGE_DECLARE_REF(window);
    MGE_DECLARE_REF(monitor);
    MGE_DECLARE_REF(render_context);
    MGE_DECLARE_REF(swap_chain);
    class hardware_buffer;
    class index_buffer;
    class vertex_buffer;
    class shader;
    class program;
    class frame_buffer;

    MGE_DECLARE_REF(texture);
    MGE_DECLARE_REF(image);
    MGE_DECLARE_REF(mesh);

    MGE_DECLARE_REF(frame_debugger);

    class uniform_base;
} // namespace mge

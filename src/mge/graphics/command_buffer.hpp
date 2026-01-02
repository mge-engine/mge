// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/dllexport.hpp"
#include "mge/graphics/vertex_buffer_handle.hpp"
#include "mge/graphics/index_buffer_handle.hpp"
#include "mge/graphics/program_handle.hpp"

namespace mge {
    class MGEGRAPHICS_EXPORT command_buffer
    {
    public:
        void draw(const program_handle&       program,
                  const vertex_buffer_handle& vertices,
                  const index_buffer_handle&  indices);
    };
} // namespace mge
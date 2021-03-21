// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
namespace mge {
    namespace win32 {
        class render_system : public mge::render_system
        {
        public:
            render_system();
            virtual ~render_system() = default;
        };
    } // namespace win32
} // namespace mge
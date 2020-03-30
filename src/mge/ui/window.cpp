// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/ui/window.hpp"
namespace mge {
    namespace ui {
        window::window(mge::ui::context &ctx) : context_object(ctx) {}

        window::~window() {}
    } // namespace ui
} // namespace mge

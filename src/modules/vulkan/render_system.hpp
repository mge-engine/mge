// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"

namespace mge::vulkan {

    class render_system : public mge::render_system
    {
    public:
        render_system();
        ~render_system();

        mge::window_ref
        create_window(const mge::extent&         extent,
                      const mge::window_options& options) override;

        std::span<mge::monitor_ref> monitors() override;
    };

} // namespace mge::vulkan
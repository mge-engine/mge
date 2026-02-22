// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
#include <memory>

namespace mge::opengl {

    class render_system : public mge::render_system
    {
    public:
        using mge::render_system::create_window;

        render_system();
        ~render_system() = default;

        mge::window_ref create_window(const mge::extent&         extent,
                                      const mge::window_options& options) override;

        std::span<mge::monitor_ref> monitors() override;

        class capabilities;

    private:
        void init_capabilities();
    };

} // namespace mge::opengl

// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
namespace mge::dx11 {

    class render_system : public mge::render_system
    {
    public:
        render_system();
        ~render_system() = default;

        std::span<mge::monitor_ref> monitors() override;

        mge::window_ref render_system::create_window(
            const mge::extent&         extent,
            const mge::window_options& options) override;

        bool debug() const;
        bool software_device() const;
    };

} // namespace mge::dx11
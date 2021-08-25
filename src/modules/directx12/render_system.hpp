// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx12.hpp"
#include "mge/graphics/render_system.hpp"

namespace mge::dx12 {
    class render_system : public mge::render_system
    {
    public:
        render_system();
        virtual ~render_system() = default;

        monitor_collection monitors();
        mge::window_ref    create_window(const mge::extent&         extent,
                                         const mge::window_options& options);

        bool debug() const;
        bool warp() const;
        bool tearing_support() const;

    private:
        bool m_tearing_support;

        void enable_debug_layer();
        void check_tearing_support();
    };
} // namespace mge::dx12
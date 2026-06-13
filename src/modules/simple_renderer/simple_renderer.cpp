// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/renderer.hpp"

namespace mge {
    MGE_DEFINE_TRACE(SMPLREND);
}

namespace mge::simplerender {

    class renderer : public mge::renderer
    {
    public:
        renderer() = default;
        ~renderer() = default;

        void render(mge::scene& scene, mge::render_context& context) override
        {
            MGE_DEBUG_TRACE(SMPLREND, "Rendering scene with Simple Renderer");
        }
    };

    MGE_REGISTER_IMPLEMENTATION(renderer, mge::renderer, simple);

} // namespace mge::simplerender

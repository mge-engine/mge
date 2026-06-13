// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"
#include "mge/core/trace.hpp"
#include "mge/scene/renderer.hpp"

namespace mge {
    MGE_DEFINE_TRACE(FWDPLUS);
}

namespace mge::forwardplus {

    class renderer : public mge::renderer
    {
    public:
        renderer() = default;
        ~renderer() = default;

        void render(mge::scene& scene, mge::render_context& context) override
        {
            MGE_DEBUG_TRACE(FWDPLUS, "Rendering scene with Forward+ renderer");
        }
    };

    MGE_REGISTER_IMPLEMENTATION(renderer, mge::renderer, forwardplus);

} // namespace mge::forwardplus

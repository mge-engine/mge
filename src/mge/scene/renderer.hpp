// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene_fwd.hpp"

namespace mge {

    /**
     * @brief A renderer is responsible for rendering the scene.
     */
    class MGESCENE_EXPORT renderer : public component<renderer>
    {
    protected:
        renderer() noexcept = default;

    public:
        virtual ~renderer() = default;

        virtual void render(scene& s, render_context& context) = 0;
    };

} // namespace mge
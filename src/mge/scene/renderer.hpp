// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/scene/camera.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/scene.hpp"

namespace mge::scene {

    /**
     * @brief Renderer that renders a scene given a camera.
     */
    class MGESCENE_EXPORT renderer : public mge::component<renderer>
    {
    public:
        virtual ~renderer() = default;

        /**
         * @brief Render the scene.
         * @param scene The scene to render.
         */
        virtual void render(const scene& scene, const camera& camera) = 0;
    };

} // namespace mge::scene
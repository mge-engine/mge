// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/entity/entity.hpp"
#include "mge/scene/dllexport.hpp"
#include "mge/scene/world.hpp"

namespace mge::scene {

    class MGESCENE_EXPORT scene : public mge::noncopyable
    {
    public:
        scene(world& w);
        ~scene();

    private:
        mge::entity::entity m_scene_entity; //!< The scene entity
    };

} // namespace mge::scene

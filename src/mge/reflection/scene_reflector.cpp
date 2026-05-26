// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/scene/node.hpp"
#include "mge/scene/scene.hpp"
#include "mge/scene/world.hpp"

namespace mge::reflection {

    class scene_reflector : public reflector
    {
    public:
        scene_reflector() = default;
        ~scene_reflector() = default;

        void reflect() const override
        {
            auto mge = module("mge");

            mge(type<mge::world>()
                    .constructor<>()
                    .method("create_scene",
                            static_cast<mge::scene (mge::world::*)(
                                std::string_view)>(&mge::world::create_scene))
                    .method("update", &mge::world::update));
            mge(type<mge::scene>()
                    .constructor<>()
                    .constructor<mge::world&>()
                    .method("create_entity",
                            static_cast<mge::entity (mge::scene::*)(void)>(
                                &mge::scene::create_entity))
                    .method("create_entity",
                            static_cast<mge::entity (mge::scene::*)(
                                std::string_view)>(&mge::scene::create_entity))
                    .method("create_node",
                            static_cast<mge::node (mge::scene::*)(void)>(
                                &mge::scene::create_node))
                    .method("create_node",
                            static_cast<mge::node (mge::scene::*)(
                                std::string_view)>(&mge::scene::create_node))
                    .method("update", &mge::scene::update));
            mge(type<mge::node>()
                    .constructor<>()
                    .constructor<mge::entity&>()
                    .method("position", &mge::node::position)
                    .method("set_position", &mge::node::set_position)
                    .method("rotation", &mge::node::rotation)
                    .method("set_rotation", &mge::node::set_rotation)
                    .method("scale", &mge::node::scale)
                    .method("set_scale", &mge::node::set_scale)
                    .method("transform", &mge::node::transform));
        }

        std::span<std::string_view> dependencies() const override
        {
            static std::array<std::string_view, 4> deps{
                {"core", "std", "graphics", "input"}};
            return deps;
        }
    };

    MGE_REGISTER_IMPLEMENTATION(scene_reflector,
                                mge::reflection::reflector,
                                scene);
} // namespace mge::reflection

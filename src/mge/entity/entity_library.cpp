// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/artifact.hpp"
#include "mge/entity/dllexport.hpp"
#include "mge/entity/entt.hpp"
namespace mge {
    MGE_ENTITY_EXPORT void mge_entity_system_dummy(void) {}
} // namespace mge

MGE_ARTIFACT(EnTT, "Michele Caini", "https://github.com/skypjack/entt",
             mge::version(ENTT_VERSION_MAJOR, ENTT_VERSION_MINOR,
                          ENTT_VERSION_PATCH),
             mge::license(mge::license_type::MIT))
    ->used_by("mge");
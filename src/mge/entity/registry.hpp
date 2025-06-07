// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/noncopyable.hpp"
#include "mge/entity/dllexport.hpp"
#include "mge/entity/flecs.hpp"

namespace mge::entity {

    class MGEENTITY_EXPORT registry : public flecs::world
    {
    public:
        registry() = default;
        ~registry() = default;
    };

} // namespace mge::entity

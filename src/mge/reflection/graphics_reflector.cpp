// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/function.hpp"
#include "mge/reflection/module.hpp"
#include "mge/reflection/reflector.hpp"
#include "mge/reflection/type.hpp"

#include "mge/graphics/topology.hpp"

namespace mge::reflection {

    class graphics_reflector : public reflector
    {
    public:
        graphics_reflector() = default;
        ~graphics_reflector() = default;
        void reflect() const override
        {
            auto mge = module("mge");
            mge(type<mge::topology>());
        }
    };

    MGE_REGISTER_IMPLEMENTATION(graphics_reflector,
                                mge::reflection::reflector,
                                graphics);
} // namespace mge::reflection

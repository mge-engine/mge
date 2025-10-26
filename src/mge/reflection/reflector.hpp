// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/component.hpp"
#include "mge/reflection/dllexport.hpp"

namespace mge::reflection {

    /**
     * A reflector exposes types and their details for reflection.
     */
    class MGEREFLECTION_EXPORT reflector : public mge::component<reflector>
    {
    public:
        reflector();
        ~reflector();

        /**
         * Expose the types to reflection.
         */
        virtual void reflect() const = 0;
    };

} // namespace mge::reflection